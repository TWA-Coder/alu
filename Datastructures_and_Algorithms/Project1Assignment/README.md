# Project 1 Assignment - Solutions and Testing Guide

This repository contains solutions for the Data Structures and Algorithms Project 1. Below are the instructions on how to compile, run, and test each solution.

## Prerequisites
- GCC Compiler (or any standard C compiler)
- Windows/Linux/macOS Terminal

---

## 1. Student Record Sorter (`sorter.c`)

This program reads student records from a file, sorts them based on grades (ascending) and first names (lexicographical order), and writes the result to a new file.

### Files
- Source Code: `sorter.c`
- Input File: `students.txt`
- Output File: `sorted_students.txt`

### How to Compile
```bash
gcc sorter.c -o sorter
```

### How to Test
1. Ensure `students.txt` exists in the same directory with student records (Format: `ID FirstName LastName Grade`).
2. Run the program:
   ```bash
   ./sorter
   ```
   *(On Windows, use `sorter.exe`)*
3. Check the console output for "Successfully wrote X records to sorted_students.txt".
4. Open `sorted_students.txt` to verify the records are sorted by grade (lowest to highest).

---

## 2. Student Lookup System - BST (`student_bst.c`)

This program loads student records into a Binary Search Tree (BST) organized by Last Name. It allows users to search for students efficiently.

### Files
- Source Code: `student_bst.c`
- Input File: `student_records.txt`

### How to Compile
```bash
gcc student_bst.c -o student_bst
```

### How to Test
1. Ensure `student_records.txt` exists.
2. Run the program:
   ```bash
   ./student_bst
   ```
3. When prompted, select Option `1` to Search.
4. Enter a Last Name (e.g., matching one in the text file).
5. Verify that:
   - If the student exists, their full details (Name, Grade) are printed.
   - If multiple students have the same Last Name, all are found.
   - If the student does not exist, a "No records found" message appears.

---

## 3. Bus Route Simulation (`bus_route.c`)

This program simulates a bus route using a Doubly Linked List, allowing navigation between stops in both directions (Home ↔ Campus).

### Files
- Source Code: `bus_route.c`

### How to Compile
```bash
gcc bus_route.c -o bus_route
```

### How to Test
1. Run the program:
   ```bash
   ./bus_route
   ```
2. **Setup**: Enter the initial number of stops (e.g., 3) and provide names and numbers for them when prompted.
3. **Menu Testing**:
   - Select `1` (Move Forward): Verify it lists stops from Home to Campus.
   - Select `2` (Move Backward): Verify it lists stops from Campus to Home.
   - Select `3` (Add New Stop): Add a new stop and verify it appears in the traversal.

---

## 4. Binary Tree Analysis (`binary_tree.c`)

This program constructs a Binary Search Tree from a specific set of 68 distinct integers and performs various analysis operations.

### Files
- Source Code: `binary_tree.c`

### How to Compile
```bash
gcc binary_tree.c -o binary_tree
```

### How to Test
1. Run the program:
   ```bash
   ./binary_tree
   ```
2. **Menu Testing**:
   - Select `1` (Print Root): Verifies the root node (first inserted value).
   - Select `2` (Print Leaves): Lists all nodes with no children.
   - Select `3` (Query Node):
     - Enter a value known to be in the tree (e.g., `500` or `15`).
     - Verify it accurately displays the Parent, Sibling, and Grandchildren.
     - Try a value *not* in the tree to verify the error handling.

---
**Note:** All source files utilize standard C libraries. If you are on Windows and see an error about `unistd.h` in `bus_route.c`, ensure the code block handling `_WIN32` is present (it is included in the solution).
