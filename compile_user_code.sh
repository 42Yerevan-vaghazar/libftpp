#!/bin/bash

# Set variables
SRC_DIR="./src"
INCLUDE_DIR="./include"
LIB_DIR="./lib"
FTPP_DIR="./libftpp_user_code"

# Compile each .cpp file in libftpp individually
for file in "$FTPP_DIR"/*.cpp; do
    # Extract the filename without the extension
    filename=$(basename "$file" .cpp)
    
    # Compile the file
    g++ -std=c++17 -I"$SRC_DIR" -I"$INCLUDE_DIR" -L"$LIB_DIR" -lftpp "$file" -o "$filename"
    
    # Check if the compilation was successful
    if [ $? -eq 0 ]; then
        echo "Compilation successful! Executable: $filename"
    else
        echo "Compilation failed for $file!"
    fi
done