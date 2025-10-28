#include "page_replacement.h"

/**
 * Print usage information
 */
void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s <algorithm>\n", program_name);
    fprintf(stderr, "Algorithms:\n");
    fprintf(stderr, "  FIFO  - First-In-First-Out\n");
    fprintf(stderr, "  OPT   - Optimal\n");
    fprintf(stderr, "  CLK   - Second Chance (Clock)\n");
    fprintf(stderr, "\nExample:\n");
    fprintf(stderr, "  %s FIFO < inputfile.csv\n", program_name);
    fprintf(stderr, "  %s OPT < inputfile.csv\n", program_name);
    fprintf(stderr, "  %s CLK < inputfile.csv\n", program_name);
}

/**
 * Main function - Entry point of the program
 */
int main(int argc, char *argv[]) {
    // Check command-line arguments
    if (argc != 2) {
        fprintf(stderr, "Error: Invalid number of arguments\n");
        print_usage(argv[0]);
        return 1;
    }
    
    // Get algorithm name
    char *algorithm = argv[1];
    
    // Validate algorithm name
    if (strcmp(algorithm, "FIFO") != 0 && 
        strcmp(algorithm, "OPT") != 0 && 
        strcmp(algorithm, "CLK") != 0) {
        fprintf(stderr, "Error: Invalid algorithm '%s'\n", algorithm);
        fprintf(stderr, "Valid algorithms: FIFO, OPT, CLK\n");
        print_usage(argv[0]);
        return 1;
    }
    
    // Read input from stdin
    InputData *data = read_input();
    
    // Check if input was read successfully
    if (data == NULL || data->count == 0) {
        fprintf(stderr, "Error: No valid input data found\n");
        if (data) {
            free_input(data);
        }
        return 1;
    }
    
    // Print input statistics
    fprintf(stderr, "Successfully read %d page references\n", data->count);
    
    // Run the appropriate algorithm
    if (strcmp(algorithm, "FIFO") == 0) {
        run_fifo_experiments(data);
    } 
    else if (strcmp(algorithm, "OPT") == 0) {
        run_optimal_experiments(data);
    } 
    else if (strcmp(algorithm, "CLK") == 0) {
        run_clock_experiments(data);
    }
    
    // Clean up
    free_input(data);
    
    return 0;
}