# Makefile for Assignment 3 - Page Replacement Algorithms
# CPSC 457 - Fall 2025

# Compiler and flags
CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lm

# Target executable
TARGET = a3

# Source files
SOURCES = a3.c utils.c fifo.c optimal.c clock.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Header files
HEADERS = page_replacement.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	@echo "Build successful! Executable: $(TARGET)"

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Run FIFO algorithm
run-fifo: $(TARGET)
	@echo "Running FIFO algorithm..."
	./$(TARGET) FIFO < Assignment_3_input_file.csv

# Run Optimal algorithm
run-opt: $(TARGET)
	@echo "Running Optimal algorithm..."
	./$(TARGET) OPT < Assignment_3_input_file.csv

# Run Second Chance (Clock) algorithm
run-clk: $(TARGET)
	@echo "Running Second Chance (Clock) algorithm..."
	./$(TARGET) CLK < Assignment_3_input_file.csv

# Run all algorithms
run-all: run-fifo run-opt run-clk

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleaned build artifacts"

# Clean everything including results
clean-all: clean
	rm -f results/data/*.csv
	@echo "Cleaned all artifacts and results"

# Create results directories if they don't exist
setup:
	mkdir -p results/data results/plots
	@echo "Created results directories"

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build the executable"
	@echo "  make all      - Build the executable (same as make)"
	@echo "  make run-fifo - Run FIFO algorithm"
	@echo "  make run-opt  - Run Optimal algorithm"
	@echo "  make run-clk  - Run Second Chance algorithm"
	@echo "  make run-all  - Run all algorithms"
	@echo "  make clean    - Remove object files and executable"
	@echo "  make clean-all- Remove all artifacts and results"
	@echo "  make setup    - Create results directories"
	@echo "  make help     - Show this help message"

# Phony targets (not actual files)
.PHONY: all clean clean-all run-fifo run-opt run-clk run-all setup help