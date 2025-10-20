#!/bin/bash

# Run for N = 5, 10, and 15 customers per ticket seller

echo "Generating output files for Project 3..."

# Clean previous output files
rm -f output_N_5.txt output_N_10.txt output_N_15.txt

# Generate output for N=5
echo "Running simulation for N=5..."
./main 5 > output_N_5.txt 2>&1
echo "Output saved to output_N_5.txt"

# Generate output for N=10
echo "Running simulation for N=10..."
./main 10 > output_N_10.txt 2>&1
echo "Output saved to output_N_10.txt"

# Generate output for N=15
echo "Running simulation for N=15..."
./main 15 > output_N_15.txt 2>&1
echo "Output saved to output_N_15.txt"

echo "All output files generated successfully!"
echo "Files created:"
echo "- output_N_5.txt"
echo "- output_N_10.txt" 
echo "- output_N_15.txt"
