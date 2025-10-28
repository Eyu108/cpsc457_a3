#include "page_replacement.h"

/**
 * Simulate Second Chance (Clock) page replacement algorithm
 * 
 * @param data: Input data containing page references
 * @param num_frames: Number of page frames available
 * @param n_bits: Number of bits in the reference register (1-32)
 * @param m_interval: Number of references between register shifts
 * @return Result structure with page faults and write-backs
 */
Result simulate_clock(InputData *data, int num_frames, int n_bits, int m_interval) {
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
    
    int clock_hand = 0;  // Clock hand position (circular queue pointer)
    int ref_counter = 0;  // Counter for reference register shifts
    
    // Process each page reference
    for (int i = 0; i < data->count; i++) {
        int page = data->references[i].page_number;
        int dirty = data->references[i].dirty;
        
        // Check if it's time to shift reference registers
        if (ref_counter >= m_interval) {
            // Shift all reference registers to the right by 1 bit
            for (int j = 0; j < num_frames; j++) {
                if (frames[j].page_number != -1) {
                    frames[j].ref_bits >>= 1;  // Right shift by 1
                }
            }
            ref_counter = 0;  // Reset counter
        }
        
        // Check if page is already in frames (page hit)
        int frame_idx = find_page_in_frames(frames, num_frames, page);
        
        if (frame_idx != -1) {
            // Page hit - page is already in memory
            // Set the high-order bit of reference register to 1
            frames[frame_idx].ref_bits |= (1U << (n_bits - 1));
            
            // Update dirty bit (if current reference is dirty, mark frame as dirty)
            if (dirty) {
                frames[frame_idx].dirty = 1;
            }
            
            ref_counter++;
            continue;
        }
        
        // Page fault - page is not in memory
        result.page_faults++;
        
        // Try to find an empty frame first
        int victim_frame = find_empty_frame(frames, num_frames);
        
        if (victim_frame == -1) {
            // No empty frames - use Second Chance algorithm
            // Search for victim starting from clock_hand position
            
            while (1) {
                // Check if current frame's reference bits are all zero
                if (frames[clock_hand].ref_bits == 0) {
                    // Found victim - all reference bits are 0
                    victim_frame = clock_hand;
                    clock_hand = (clock_hand + 1) % num_frames;  // Move clock hand
                    break;
                }
                
                // Give second chance: shift reference register right by 1
                frames[clock_hand].ref_bits >>= 1;
                
                // Move clock hand to next frame (circular)
                clock_hand = (clock_hand + 1) % num_frames;
            }
            
            // Check if victim page is dirty (needs write-back)
            if (frames[victim_frame].dirty) {
                result.write_backs++;
            }
        } else {
            // Found empty frame, but update clock_hand if we used it
            // This helps maintain circular order for FIFO tie-breaking
            if (victim_frame == clock_hand) {
                clock_hand = (clock_hand + 1) % num_frames;
            }
        }
        
        // Load new page into the victim frame
        frames[victim_frame].page_number = page;
        frames[victim_frame].dirty = dirty;
        
        // Set the high-order bit of reference register to 1 (just referenced)
        frames[victim_frame].ref_bits = (1U << (n_bits - 1));
        
        ref_counter++;
    }
    
    // Free allocated memory
    free(frames);
    
    return result;
}

/**
 * Run Second Chance experiments for Part 2
 * Two sets of experiments:
 * 1. Fix m=10, vary n from 1 to 32 with 50 frames
 * 2. Fix n=8, vary m from 1 to 100 with 50 frames
 */
void run_clock_experiments(InputData *data) {
    const int FRAMES = 50;
    
    // Experiment 1: Fix m=10, vary n from 1 to 32
    {
        const int M_FIXED = 10;
        const int MIN_N = 1;
        const int MAX_N = 32;
        int num_experiments = MAX_N - MIN_N + 1;
        
        Result *results = (Result*)malloc(num_experiments * sizeof(Result));
        if (!results) {
            fprintf(stderr, "Error: Memory allocation failed for results\n");
            exit(1);
        }
        
        // Print table header
        printf("\nCLK, m=%d\n", M_FIXED);
        printf("+----------+----------------+-----------------+\n");
        printf("| %-8s | %-14s | %-15s |\n", "n", "Page Faults", "Write-backs");
        printf("+----------+----------------+-----------------+\n");
        
        // Run experiments
        for (int n = MIN_N; n <= MAX_N; n++) {
            Result result = simulate_clock(data, FRAMES, n, M_FIXED);
            result.frames = n;  // Store n value for CSV
            results[n - MIN_N] = result;
            
            // Print result row
            print_table_row(n, result.page_faults, result.write_backs);
        }
        
        // Save results to CSV
        save_results_to_csv("clock_vary_n.csv", results, num_experiments, "n");
        
        free(results);
    }
    
    // Experiment 2: Fix n=8, vary m from 1 to 100
    {
        const int N_FIXED = 8;
        const int MIN_M = 1;
        const int MAX_M = 100;
        int num_experiments = MAX_M - MIN_M + 1;
        
        Result *results = (Result*)malloc(num_experiments * sizeof(Result));
        if (!results) {
            fprintf(stderr, "Error: Memory allocation failed for results\n");
            exit(1);
        }
        
        // Print table header
        printf("\nCLK, n=%d\n", N_FIXED);
        printf("+----------+----------------+-----------------+\n");
        printf("| %-8s | %-14s | %-15s |\n", "m", "Page Faults", "Write-backs");
        printf("+----------+----------------+-----------------+\n");
        
        // Run experiments
        for (int m = MIN_M; m <= MAX_M; m++) {
            Result result = simulate_clock(data, FRAMES, N_FIXED, m);
            result.frames = m;  // Store m value for CSV
            results[m - MIN_M] = result;
            
            // Print result row
            print_table_row(m, result.page_faults, result.write_backs);
        }
        
        // Save results to CSV
        save_results_to_csv("clock_vary_m.csv", results, num_experiments, "m");
        
        free(results);
    }
}