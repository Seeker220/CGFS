#!/bin/bash

echo "Compiling project..."

# Compile main.cpp into an executable named 'cgfs_run'
g++ -std=c++23 -o cgfs_run main.cpp

# Check if the compilation command succeeded ($? is the exit code)
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running program..."
    echo "============================================"
    ./cgfs_run
else
    echo "Compilation failed."
fi