#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Constants
#define MAX_PAGES 500           // Maximum number of different pages (0-499)
#define MAX_REFERENCES 20000    // Maximum number of page references
#define MAX_LINE_LENGTH 256     // Maximum length of input line

// Page reference structure
typedef struct {
    int page_number;    // Page number (0-499)
    int dirty;          // Dirty bit (0 or 1)
} PageReference;

// Frame structure for page replacement algorithms
typedef struct {
    int page_number;    // Page number stored in this frame (-1 if empty)
    int dirty;          // Dirty bit for this page
    int load_time;      // Time when page was loaded (for FIFO)
    int last_access;    // Last access time (for LRU-like algorithms)
    uint32_t ref_bits;  // Reference bits for Second Chance (n-bit register)
} Frame;

// Result structure to store algorithm results
typedef struct {
    int frames;         // Number of frames used
    int page_faults;    // Total page faults
    int write_backs;    // Total write-backs
} Result;

// Input data structure
typedef struct {
    PageReference *references;  // Array of page references
    int count;                   // Number of references
} InputData;

// Function prototypes - Utils
InputData* read_input(void);
void free_input(InputData *data);
void print_table_header(const char *algorithm_name);
void print_table_row(int param, int page_faults, int write_backs);
void print_table_footer(void);
void save_results_to_csv(const char *filename, Result *results, int count, const char *param_name);

// Function prototypes - FIFO Algorithm
Result simulate_fifo(InputData *data, int num_frames);
void run_fifo_experiments(InputData *data);

// Function prototypes - Optimal Algorithm
Result simulate_optimal(InputData *data, int num_frames);
void run_optimal_experiments(InputData *data);

// Function prototypes - Second Chance (Clock) Algorithm
Result simulate_clock(InputData *data, int num_frames, int n_bits, int m_interval);
void run_clock_experiments(InputData *data);

// Helper functions
int find_page_in_frames(Frame *frames, int num_frames, int page_number);
int find_empty_frame(Frame *frames, int num_frames);
void initialize_frames(Frame *frames, int num_frames);

#endif // PAGE_REPLACEMENT_H