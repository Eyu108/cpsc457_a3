#include "page_replacement.h"

/**
 * Find the next use of a page starting from current position
 * Returns the index of next use, or -1 if page is never used again
 */
static int find_next_use(InputData *data, int current_pos, int page_number) {
    for (int i = current_pos + 1; i < data->count; i++) {
        if (data->references[i].page_number == page_number) {
            return i;
        }
    }
    return -1;  // Page will never be used again
}

/**
 * Find the victim frame for optimal replacement
 * Returns the frame index of the page that will be used farthest in the future
 * (or never used again)
 */
static int find_optimal_victim(Frame *frames, int num_frames, InputData *data, int current_pos) {
    int victim_frame = 0;
    int farthest_use = -1;
    
    for (int i = 0; i < num_frames; i++) {
        int page = frames[i].page_number;
        
        // Find when this page will be used next
        int next_use = find_next_use(data, current_pos, page);
        
        // If page is never used again, choose it immediately
        if (next_use == -1) {
            return i;
        }
        
        // Track the page with the farthest next use
        if (next_use > farthest_use) {
            farthest_use = next_use;
            victim_frame = i;
        }
    }
    
    return victim_frame;
}

/**
 * Simulate Optimal page replacement algorithm
 * 
 * @param data: Input data containing page references
 * @param num_frames: Number of page frames available
 * @return Result structure with page faults and write-backs
 */
Result simulate_optimal(InputData *data, int num_frames) {
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
            // No empty frames - need to evict using optimal strategy
            // Replace the page that will be used farthest in the future
            victim_frame = find_optimal_victim(frames, num_frames, data, i);
            
            // Check if victim page is dirty (needs write-back)
            if (frames[victim_frame].dirty) {
                result.write_backs++;
            }
        }
        
        // Load new page into the victim frame
        frames[victim_frame].page_number = page;
        frames[victim_frame].dirty = dirty;
    }
    
    // Free allocated memory
    free(frames);
    
    return result;
}

/**
 * Run Optimal experiments for frames 1 to 100
 * Print results and save to CSV
 */
void run_optimal_experiments(InputData *data) {
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
    print_table_header("OPT");
    
    // Run simulation for each frame count
    for (int frames = MIN_FRAMES; frames <= MAX_FRAMES; frames++) {
        Result result = simulate_optimal(data, frames);
        results[frames - MIN_FRAMES] = result;
        
        // Print result row
        print_table_row(frames, result.page_faults, result.write_backs);
    }
    
    // Save results to CSV for plotting
    save_results_to_csv("optimal_results.csv", results, num_experiments, "Frames");
    
    // Free results array
    free(results);
}