#!/bin/bash

# Check if g++ is installed
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ compiler not found. Please install g++."
    exit 1
fi

# Check if main.cpp exists
if [ ! -f "main.cpp" ]; then
    echo "Error: main_program.cpp not found in current directory."
    exit 1
fi

# Check if test_data.txt exists
if [ ! -f "test_data.txt" ]; then
    echo "Error: test_data.txt not found. Please generate it using run_all.sh."
    exit 1
fi

# Compile main_program.cpp
echo "Compiling main.cpp..."
g++ -std=c++17 main.cpp -o main
if [ $? -ne 0 ]; then
    echo "Error: Compilation of main_program.cpp failed."
    exit 1
fi

# Run main_program
echo "Running main..."
./main
if [ $? -ne 0 ]; then
    echo "Error: main_program execution failed."
    exit 1
fi

echo "Execution complete. Results saved in Student-Oriented_result.txt and College-Oriented_result.txt."