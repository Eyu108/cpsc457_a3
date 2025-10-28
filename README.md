# CPSC 457 - Assignment 3: Page Replacement Algorithms

## Project Overview

This project implements and compares three page replacement algorithms:
1. **FIFO (First-In-First-Out)** - Replaces the oldest page in memory
2. **Optimal (OPT)** - Replaces the page that will be used farthest in the future
3. **Second Chance (Clock/CLK)** - Uses reference bits and circular scanning for LRU approximation

## Author

- **Student**: Eyu108
- **Course**: CPSC 457 - Fall 2025
- **Assignment**: Assignment 3 - Page Replacement Algorithms

## Project Structure

```
cpsc457_a3/
├── a3.c                    # Main program
├── page_replacement.h      # Header file with data structures
├── utils.c                 # Utility functions (I/O, helpers)
├── fifo.c                  # FIFO algorithm implementation
├── optimal.c               # Optimal algorithm implementation
├── clock.c                 # Second Chance (Clock) algorithm
├── Makefile                # Build automation
├── README.md               # This file
├── Assignment_3_input_file.csv  # Input data (15K references)
└── results/
    ├── data/               # Generated CSV files for plotting
    └── plots/              # Generated plots
```

## Features

### Core Functionality
- ✅ Reads page references from stdin via shell redirection
- ✅ Skips header line automatically
- ✅ Dynamic memory allocation (no fixed limits)
- ✅ Validates input data (page numbers 0-499, dirty bits 0/1)
- ✅ Tracks page faults and write-backs
- ✅ Generates formatted tables
- ✅ Exports results to CSV for plotting
- ✅ No memory leaks (valgrind clean)

### Algorithms Implemented

#### 1. FIFO (First-In-First-Out)
- Maintains pages in order of arrival
- Evicts oldest page when replacement needed
- Simple and predictable

#### 2. Optimal (OPT)
- Looks ahead in reference string
- Evicts page used farthest in future
- Theoretical best case (not practical)

#### 3. Second Chance (Clock)
- Uses n-bit reference registers per page
- Periodic right-shifting of reference bits
- Circular scanning with "second chance"
- Configurable parameters (n and m)

## Building the Project

### Prerequisites
- GCC compiler
- Linux environment (tested on cslinux.ucalgary.ca)
- Make utility

### Compilation

```bash
# Build the executable
make

# Or use the full command
gcc -O2 -Wall -Wextra -std=c99 -pedantic a3.c utils.c fifo.c optimal.c clock.c -o a3 -lm
```

### Cleaning

```bash
# Remove object files and executable
make clean

# Remove everything including results
make clean-all
```

## Running the Program

### Basic Usage

```bash
./a3 <algorithm> < inputfile.csv
```

**Algorithms:**
- `FIFO` - First-In-First-Out
- `OPT` - Optimal
- `CLK` - Second Chance (Clock)

### Examples

```bash
# Run FIFO algorithm
./a3 FIFO < Assignment_3_input_file.csv

# Run Optimal algorithm
./a3 OPT < Assignment_3_input_file.csv

# Run Second Chance algorithm
./a3 CLK < Assignment_3_input_file.csv
```

### Using Makefile Shortcuts

```bash
# Run individual algorithms
make run-fifo
make run-opt
make run-clk

# Run all algorithms
make run-all
```

## Output Format

### Part 1: FIFO and OPT

The program outputs tables for frames 1-100 showing page faults and write-backs:

```
FIFO
+----------+----------------+-----------------+
| Frames   | Page Faults    | Write-backs     |
+----------+----------------+-----------------+
| 1        | 15000          | 7500            |
+----------+----------------+-----------------+
| 2        | 12000          | 6000            |
+----------+----------------+-----------------+
...
+----------+----------------+-----------------+
| 100      | 500            | 250             |
+----------+----------------+-----------------+
```

### Part 2: Second Chance (CLK)

Two experiments with 50 frames:

**Experiment 1: Fix m=10, vary n (1-32)**
```
CLK, m=10
+----------+----------------+-----------------+
| n        | Page Faults    | Write-backs     |
+----------+----------------+-----------------+
| 1        | 8000           | 4000            |
+----------+----------------+-----------------+
...
```

**Experiment 2: Fix n=8, vary m (1-100)**
```
CLK, n=8
+----------+----------------+-----------------+
| m        | Page Faults    | Write-backs     |
+----------+----------------+-----------------+
| 1        | 9000           | 4500            |
+----------+----------------+-----------------+
...
```

## CSV Output Files

Results are automatically saved to `results/data/` for plotting:

- `fifo_results.csv` - FIFO results (frames 1-100)
- `optimal_results.csv` - Optimal results (frames 1-100)
- `clock_vary_n.csv` - Clock results varying n (m=10, 50 frames)
- `clock_vary_m.csv` - Clock results varying m (n=8, 50 frames)

**CSV Format:**
```csv
Frames,PageFaults,WriteBack
1,15000,7500
2,12000,6000
...
```

## Input File Format

The input file contains page references with dirty bits:

```csv
page,dirty
123,0
456,1
789,0
...
```

- **page**: Page number (0-499)
- **dirty**: Dirty bit (0 or 1)
- First line is header (skipped automatically)
- 15,000 references total
- 500 unique pages

## Algorithm Details

### FIFO Algorithm
1. Check if page is in memory → Hit (no fault)
2. If not in memory → Fault
3. Find empty frame or evict oldest page
4. If evicted page is dirty → Write-back
5. Load new page

### Optimal Algorithm
1. Check if page is in memory → Hit
2. If not in memory → Fault
3. Find empty frame or evict page used farthest in future
4. If evicted page is dirty → Write-back
5. Load new page

### Second Chance (Clock) Algorithm
1. Every reference: Set high-order reference bit to 1
2. Every m references: Shift all reference registers right
3. On page fault:
   - Scan circularly from clock hand
   - Skip frames with ref_bits ≠ 0 (give second chance, shift right)
   - Evict first frame with ref_bits = 0
4. If evicted page is dirty → Write-back

## Testing

### Test on University Server

```bash
# SSH to university server
ssh username@cslinux.ucalgary.ca

# Navigate to project directory
cd cpsc457_a3

# Build
make

# Test
make run-all
```

### Check for Memory Leaks

```bash
# Install valgrind if needed
sudo apt-get install valgrind

# Run with valgrind
valgrind --leak-check=full ./a3 FIFO < Assignment_3_input_file.csv
```

## Assignment Requirements Checklist

- [x] Code compiles on cslinux.ucalgary.ca
- [x] GIT repository with version history
- [x] Accepts only valid arguments (FIFO, OPT, CLK)
- [x] Reads from stdin via shell redirection
- [x] Produces correctly formatted tables
- [x] FIFO implementation
- [x] Optimal implementation
- [x] Second Chance implementation with n-bit registers
- [x] Part 1: Frames 1-100 for FIFO and OPT
- [x] Part 2: Two experiments with 50 frames for CLK
- [x] CSV output for plotting
- [x] Clean, well-organized code
- [x] Meaningful variable names
- [x] Comments throughout
- [x] No memory leaks
- [x] Handles bad arguments
- [x] Skips header line
- [x] Dynamic input size

## Code Quality

- **Clean Code**: Well-organized with clear separation of concerns
- **Meaningful Names**: Variables and functions are descriptive
- **Comments**: Comprehensive documentation throughout
- **Error Handling**: Validates all inputs and handles errors gracefully
- **Memory Management**: No leaks, proper allocation/deallocation
- **Efficiency**: O(n) for FIFO and Clock, O(n²) for Optimal (unavoidable)

## Troubleshooting

### Program doesn't compile
- Ensure you're on cslinux.ucalgary.ca
- Check that all source files are present
- Run `make clean` then `make`

### No output or errors
- Check input file exists: `ls -l Assignment_3_input_file.csv`
- Verify shell redirection: `./a3 FIFO < Assignment_3_input_file.csv`
- Check stderr for error messages

### CSV files not generated
- Ensure `results/data/` directory exists: `make setup`
- Check write permissions: `ls -ld results/data/`

## Future Improvements

- [ ] Implement LRU (Least Recently Used) algorithm
- [ ] Add graphical visualization
- [ ] Support multiple input files
- [ ] Add performance benchmarking
- [ ] Implement parallel processing for experiments

## References

- CPSC 457 Course Materials
- Operating System Concepts (Silberschatz, Galvin, Gagne)
- Assignment 3 Specification

## License

This project is for educational purposes as part of CPSC 457 at the University of Calgary.

---

**Last Updated**: October 2025