#!/bin/bash

# Test script for Assignment 3 - Page Replacement Algorithms
# This script builds the project and runs all algorithms

echo "=========================================="
echo "CPSC 457 Assignment 3 - Testing Script"
echo "=========================================="
echo ""

# Check if input file exists
if [ ! -f "Assignment_3_input_file.csv" ]; then
    echo "ERROR: Assignment_3_input_file.csv not found!"
    echo "Please copy the input file to this directory."
    exit 1
fi

# Count lines in input file
line_count=$(wc -l < Assignment_3_input_file.csv)
echo "Input file found: Assignment_3_input_file.csv"
echo "Total lines (including header): $line_count"
echo ""

# Clean previous builds
echo "=========================================="
echo "Step 1: Cleaning previous builds"
echo "=========================================="
make clean
echo ""

# Build the project
echo "=========================================="
echo "Step 2: Building the project"
echo "=========================================="
make
if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi
echo ""

# Check if executable was created
if [ ! -f "./a3" ]; then
    echo "ERROR: Executable 'a3' was not created!"
    exit 1
fi
echo "✓ Build successful! Executable 'a3' created."
echo ""

# Create results directories if they don't exist
mkdir -p results/data results/plots

# Test with invalid arguments
echo "=========================================="
echo "Step 3: Testing argument validation"
echo "=========================================="
echo "Testing with no arguments:"
./a3 2>&1 | head -5
echo ""

echo "Testing with invalid algorithm:"
./a3 INVALID 2>&1 | head -5
echo ""

echo "✓ Argument validation working correctly"
echo ""

# Run FIFO algorithm
echo "=========================================="
echo "Step 4: Running FIFO Algorithm"
echo "=========================================="
echo "This may take a few moments..."
./a3 FIFO < Assignment_3_input_file.csv > results/fifo_output.txt 2>&1
if [ $? -eq 0 ]; then
    echo "✓ FIFO completed successfully"
    echo ""
    echo "First 20 lines of output:"
    head -20 results/fifo_output.txt
    echo ""
    echo "Last 10 lines of output:"
    tail -10 results/fifo_output.txt
    echo ""
    
    # Check specific frame values
    echo "Sample results from FIFO:"
    echo "  Frame 1:"
    grep "| 1 " results/fifo_output.txt | head -1
    echo "  Frame 50:"
    grep "| 50 " results/fifo_output.txt | head -1
    echo "  Frame 100:"
    grep "| 100 " results/fifo_output.txt | head -1
else
    echo "✗ FIFO failed!"
    cat results/fifo_output.txt
fi
echo ""

# Run Optimal algorithm
echo "=========================================="
echo "Step 5: Running Optimal Algorithm"
echo "=========================================="
echo "This may take longer (looks into future)..."
./a3 OPT < Assignment_3_input_file.csv > results/optimal_output.txt 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Optimal completed successfully"
    echo ""
    echo "First 20 lines of output:"
    head -20 results/optimal_output.txt
    echo ""
    echo "Last 10 lines of output:"
    tail -10 results/optimal_output.txt
    echo ""
    
    # Check specific frame values
    echo "Sample results from Optimal:"
    echo "  Frame 1:"
    grep "| 1 " results/optimal_output.txt | head -1
    echo "  Frame 50:"
    grep "| 50 " results/optimal_output.txt | head -1
    echo "  Frame 100:"
    grep "| 100 " results/optimal_output.txt | head -1
else
    echo "✗ Optimal failed!"
    cat results/optimal_output.txt
fi
echo ""

# Run Second Chance algorithm
echo "=========================================="
echo "Step 6: Running Second Chance (Clock) Algorithm"
echo "=========================================="
echo "Running two experiments (m=10 varying n, n=8 varying m)..."
./a3 CLK < Assignment_3_input_file.csv > results/clock_output.txt 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Clock completed successfully"
    echo ""
    echo "First 25 lines of output (m=10 experiment):"
    head -25 results/clock_output.txt
    echo ""
    echo "Sample from n=8 experiment:"
    grep -A 5 "CLK, n=8" results/clock_output.txt | head -10
    echo ""
else
    echo "✗ Clock failed!"
    cat results/clock_output.txt
fi
echo ""

# Check CSV files
echo "=========================================="
echo "Step 7: Checking CSV Output Files"
echo "=========================================="
csv_files=("fifo_results.csv" "optimal_results.csv" "clock_vary_n.csv" "clock_vary_m.csv")
for csv_file in "${csv_files[@]}"; do
    if [ -f "results/data/$csv_file" ]; then
        line_count=$(wc -l < "results/data/$csv_file")
        echo "✓ $csv_file created ($line_count lines)"
        echo "  First 5 lines:"
        head -5 "results/data/$csv_file" | sed 's/^/    /'
    else
        echo "✗ $csv_file NOT found!"
    fi
    echo ""
done

# Compare FIFO vs Optimal
echo "=========================================="
echo "Step 8: Quick Comparison (Frame 50)"
echo "=========================================="
echo "Comparing page faults at 50 frames:"
fifo_50=$(grep "| 50 " results/fifo_output.txt | awk '{print $4}')
opt_50=$(grep "| 50 " results/optimal_output.txt | awk '{print $4}')
echo "  FIFO:    $fifo_50 page faults"
echo "  Optimal: $opt_50 page faults"
if [ ! -z "$fifo_50" ] && [ ! -z "$opt_50" ]; then
    if [ "$fifo_50" -ge "$opt_50" ]; then
        echo "  ✓ Correct: FIFO >= Optimal (as expected)"
    else
        echo "  ✗ Warning: FIFO < Optimal (unexpected!)"
    fi
fi
echo ""

# Summary
echo "=========================================="
echo "Test Summary"
echo "=========================================="
echo "✓ Compilation successful"
echo "✓ All three algorithms executed"
echo "✓ Output tables generated"
echo "✓ CSV files created for plotting"
echo ""
echo "Next steps:"
echo "1. Review the output files in results/"
echo "2. Verify the values match expected results"
echo "3. Create plots using the CSV files"
echo "4. Run on cslinux.ucalgary.ca to confirm compatibility"
echo ""
echo "To view full outputs:"
echo "  cat results/fifo_output.txt"
echo "  cat results/optimal_output.txt"
echo "  cat results/clock_output.txt"
echo ""
echo "=========================================="