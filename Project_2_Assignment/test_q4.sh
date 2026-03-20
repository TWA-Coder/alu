#!/bin/bash

# Compile the C program
gcc -O2 q4_search.c -o search -lpthread

echo "Generating 10 test files (dummy_1.txt to dummy_10.txt) heavily populated with text..."

for i in {1..10}; do
    # Create ~5MB of random bytes converted to alphanumeric text
    cat /dev/urandom | LC_ALL=C tr -dc 'a-zA-Z0-9 ' | fold -w 100 | head -n 50000 > dummy_$i.txt
    
    # Randomly inject the keyword 'system' a few times
    for j in {1..20}; do
        echo "system" >> dummy_$i.txt
    done
done

echo "Running benchmarks..."
echo "========================================="
echo "Test 1: 2 Threads"
./search system output_2_threads.txt dummy_1.txt dummy_2.txt dummy_3.txt dummy_4.txt dummy_5.txt dummy_6.txt dummy_7.txt dummy_8.txt dummy_9.txt dummy_10.txt 2

echo "Test 2: Average CPU Cores (4 Threads)"
./search system output_4_threads.txt dummy_1.txt dummy_2.txt dummy_3.txt dummy_4.txt dummy_5.txt dummy_6.txt dummy_7.txt dummy_8.txt dummy_9.txt dummy_10.txt 4

echo "Test 3: Maximum Threads (10 Threads = 1 Thread per File)"
./search system output_10_threads.txt dummy_1.txt dummy_2.txt dummy_3.txt dummy_4.txt dummy_5.txt dummy_6.txt dummy_7.txt dummy_8.txt dummy_9.txt dummy_10.txt 10

echo "Output of Test 3 (10_threads.txt):"
cat output_10_threads.txt
