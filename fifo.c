#include "page_replacement.h"

/**
 * Simulate FIFO page replacement algorithm
 * 
 * @param data: Input data containing page references
 * @param num_frames: Number of page frames available
 * @return Result structure with page faults and write-backs
 */
Result simulate_fifo(InputData *data, int num_frames) {
    Result result;
    result.frames = num_frames;
    result.page_faults = 0;
    result.write_backs = 0;
    
    // Allocate frames
    Frame *frames = (Frame*)malloc(num_frames * sizeof(Frame));
    if (!frames) {
        fprintf(stderr, "Error: Memory allocation failed for frames\n");
        exit(1);
    }
    
    // Initialize all frames to empty
    initialize_frames(frames, num_frames);
    
    int time = 0;  // Current time for load_time tracking
    
    // Process each page reference
    for (int i = 0; i < data->count; i++) {
        int page = data->references[i].page_number;
        int dirty = data->references[i].dirty;
        
        // Check if page is already in frames (page hit)
        int frame_idx = find_page_in_frames(frames, num_frames, page);
        
        if (frame_idx != -1) {
            // Page hit - page is already in memory
            // Update dirty bit (if current reference is dirty, mark frame as dirty)
            if (dirty) {
                frames[frame_idx].dirty = 1;
            }
            // No page fault, continue to next reference
            continue;
        }
        
        // Page fault - page is not in memory
        result.page_faults++;
        
        // Try to find an empty frame first
        int victim_frame = find_empty_frame(frames, num_frames);
        
        if (victim_frame == -1) {
            // No empty frames - need to evict using FIFO
            // Find the oldest page (minimum load_time)
            int oldest_time = frames[0].load_time;
            victim_frame = 0;
            
            for (int j = 1; j < num_frames; j++) {
                if (frames[j].load_time < oldest_time) {
                    oldest_time = frames[j].load_time;
                    victim_frame = j;
                }
            }
            
            // Check if victim page is dirty (needs write-back)
            if (frames[victim_frame].dirty) {
                result.write_backs++;
            }
        }
        
        // Load new page into the victim frame
        frames[victim_frame].page_number = page;
        frames[victim_frame].dirty = dirty;
        frames[victim_frame].load_time = time;
        
        time++;
    }
    
    // Free allocated memory
    free(frames);
    
    return result;
}

/**
 * Run FIFO experiments for frames 1 to 100
 * Print results and save to CSV
 */
void run_fifo_experiments(InputData *data) {
    const int MIN_FRAMES = 1;
    const int MAX_FRAMES = 100;
    int num_experiments = MAX_FRAMES - MIN_FRAMES + 1;
    
    // Allocate array to store all results
    Result *results = (Result*)malloc(num_experiments * sizeof(Result));
    if (!results) {
        fprintf(stderr, "Error: Memory allocation failed for results\n");
        exit(1);
    }
    
    // Print table header
    print_table_header("FIFO");
    
    // Run simulation for each frame count
    for (int frames = MIN_FRAMES; frames <= MAX_FRAMES; frames++) {
        Result result = simulate_fifo(data, frames);
        results[frames - MIN_FRAMES] = result;
        
        // Print result row
        print_table_row(frames, result.page_faults, result.write_backs);
    }
    
    // Save results to CSV for plotting
    save_results_to_csv("fifo_results.csv", results, num_experiments, "Frames");
    
    // Free results array
    free(results);
}