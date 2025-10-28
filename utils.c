#include "page_replacement.h"

/**
 * Read input from stdin (via shell redirection)
 * Format: <page#, dirty?>
 * Skips header line and handles dynamic input size
 */
InputData* read_input(void) {
    InputData *data = (InputData*)malloc(sizeof(InputData));
    if (!data) {
        fprintf(stderr, "Error: Memory allocation failed for InputData\n");
        exit(1);
    }
    
    // Allocate initial array for references
    int capacity = 1000;
    data->references = (PageReference*)malloc(capacity * sizeof(PageReference));
    if (!data->references) {
        fprintf(stderr, "Error: Memory allocation failed for references\n");
        free(data);
        exit(1);
    }
    
    data->count = 0;
    char line[MAX_LINE_LENGTH];
    bool first_line = true;
    
    // Read from stdin
    while (fgets(line, sizeof(line), stdin)) {
        // Skip header line
        if (first_line) {
            first_line = false;
            continue;
        }
        
        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }
        
        // Expand array if needed
        if (data->count >= capacity) {
            capacity *= 2;
            PageReference *temp = (PageReference*)realloc(data->references, 
                                                          capacity * sizeof(PageReference));
            if (!temp) {
                fprintf(stderr, "Error: Memory reallocation failed\n");
                free(data->references);
                free(data);
                exit(1);
            }
            data->references = temp;
        }
        
        // Parse line: <page#, dirty?>
        int page_num, dirty;
        if (sscanf(line, "%d,%d", &page_num, &dirty) == 2) {
            // Validate input
            if (page_num < 0 || page_num >= MAX_PAGES) {
                fprintf(stderr, "Warning: Invalid page number %d, skipping\n", page_num);
                continue;
            }
            if (dirty != 0 && dirty != 1) {
                fprintf(stderr, "Warning: Invalid dirty bit %d for page %d, skipping\n", 
                        dirty, page_num);
                continue;
            }
            
            data->references[data->count].page_number = page_num;
            data->references[data->count].dirty = dirty;
            data->count++;
        }
    }
    
    // Shrink array to actual size to save memory
    if (data->count < capacity) {
        PageReference *temp = (PageReference*)realloc(data->references, 
                                                      data->count * sizeof(PageReference));
        if (temp) {
            data->references = temp;
        }
    }
    
    return data;
}

/**
 * Free memory allocated for input data
 */
void free_input(InputData *data) {
    if (data) {
        if (data->references) {
            free(data->references);
        }
        free(data);
    }
}

/**
 * Print table header with algorithm name
 */
void print_table_header(const char *algorithm_name) {
    printf("\n%s\n", algorithm_name);
    printf("+----------+----------------+-----------------+\n");
    printf("| %-8s | %-14s | %-15s |\n", "Frames", "Page Faults", "Write-backs");
    printf("+----------+----------------+-----------------+\n");
}

/**
 * Print a single row in the table
 */
void print_table_row(int param, int page_faults, int write_backs) {
    printf("| %-8d | %-14d | %-15d |\n", param, page_faults, write_backs);
    printf("+----------+----------------+-----------------+\n");
}

/**
 * Print table footer (same as divider)
 */
void print_table_footer(void) {
    // Footer is printed with each row, so this function is optional
    // Keeping it for consistency if needed later
}

/**
 * Save results to CSV file for plotting
 */
void save_results_to_csv(const char *filename, Result *results, int count, 
                         const char *param_name) {
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "results/data/%s", filename);
    
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        fprintf(stderr, "Warning: Could not open %s for writing\n", filepath);
        return;
    }
    
    // Write header
    fprintf(fp, "%s,PageFaults,WriteBack\n", param_name);
    
    // Write data
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%d,%d\n", 
                results[i].frames, 
                results[i].page_faults, 
                results[i].write_backs);
    }
    
    fclose(fp);
}

/**
 * Find if a page is already in frames
 * Returns frame index if found, -1 otherwise
 */
int find_page_in_frames(Frame *frames, int num_frames, int page_number) {
    for (int i = 0; i < num_frames; i++) {
        if (frames[i].page_number == page_number) {
            return i;
        }
    }
    return -1;
}

/**
 * Find an empty frame
 * Returns frame index if found, -1 if all frames are occupied
 */
int find_empty_frame(Frame *frames, int num_frames) {
    for (int i = 0; i < num_frames; i++) {
        if (frames[i].page_number == -1) {
            return i;
        }
    }
    return -1;
}

/**
 * Initialize all frames to empty state
 */
void initialize_frames(Frame *frames, int num_frames) {
    for (int i = 0; i < num_frames; i++) {
        frames[i].page_number = -1;
        frames[i].dirty = 0;
        frames[i].load_time = 0;
        frames[i].last_access = 0;
        frames[i].ref_bits = 0;
    }
}