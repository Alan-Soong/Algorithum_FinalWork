#!/bin/bash

# Parse command line arguments
NUM_STUDENTS=""
NUM_COLLEGES=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --students)
            NUM_STUDENTS="$2"
            shift 2
            ;;
        --colleges)
            NUM_COLLEGES="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 --students <num> --colleges <num>"
            exit 1
            ;;
    esac
done

# Validate input
if [ -z "$NUM_STUDENTS" ] || ! [[ "$NUM_STUDENTS" =~ ^[0-9]+$ ]] || [ "$NUM_STUDENTS" -lt 1 ]; then
    echo "Error: --students must be a positive integer"
    exit 1
fi
if [ -z "$NUM_COLLEGES" ] || ! [[ "$NUM_COLLEGES" =~ ^[0-9]+$ ]] || [ "$NUM_COLLEGES" -lt 1 ]; then
    echo "Error: --colleges must be a positive integer"
    exit 1
fi

# Check if g++ is installed
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ compiler not found. Please install g++."
    exit 1
fi

# Check if source files exist
if [ ! -f "generate_data.cpp" ] || [ ! -f "main.cpp" ]; then
    echo "Error: generate_data.cpp or main.cpp not found in current directory."
    exit 1
fi

# Compile generate_data.cpp
echo "Compiling generate_data.cpp..."
g++ -std=c++17 generate_data.cpp -o generate_data.exe
if [ $? -ne 0 ]; then
    echo "Error: Compilation of generate_data.cpp failed."
    exit 1
fi

# Modify data scale in generate_data.cpp
echo "Modifying generate_data.cpp with numStudents=$NUM_STUDENTS and numColleges=$NUM_COLLEGES..."
cp generate_data.cpp generate_data.cpp.bak
sed -i "s/const int numStudents = [0-9]\+;/const int numStudents = $NUM_STUDENTS;/" generate_data.cpp
sed -i "s/const int numColleges = [0-9]\+;/const int numColleges = $NUM_COLLEGES;/" generate_data.cpp

# Recompile generate_data.cpp
echo "Recompiling generate_data.cpp..."
g++ -std=c++17 generate_data.cpp -o generate_data.exe
if [ $? -ne 0 ]; then
    echo "Error: Recompilation of generate_data.cpp failed."
    mv generate_data.cpp.bak generate_data.cpp
    exit 1
fi

# Run generate_data
echo "Running generate_data..."
./generate_data.exe
if [ $? -ne 0 ]; then
    echo "Error: generate_data execution failed."
    mv generate_data.cpp.bak generate_data.cpp
    exit 1
fi

# Restore original generate_data.cpp
mv generate_data.cpp.bak generate_data.cpp

# Compile main_program.cpp
echo "Compiling main_program.cpp..."
g++ -std=c++17 main.cpp -o main.exe
if [ $? -ne 0 ]; then
    echo "Error: Compilation of main.cpp failed."
    exit 1
fi

# Run main_program
echo "Running main..."
./main.exe
if [ $? -ne 0 ]; then
    echo "Error: main execution failed."
    exit 1
fi

echo "Execution complete. Results saved in test_data.txt, Student-Oriented_result.txt, and College-Oriented_result.txt."