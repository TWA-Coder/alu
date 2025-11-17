// main.h
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


// Function pointer type for operations
typedef void (*OperationFunc)(int*, int);

// Function prototypes
int* create_dataset(int size);
void free_dataset(int* data);
void compute_sum(int* data, int size);
void compute_average(int* data, int size);
void find_max_min(int* data, int size);
void sort(int* data, int size);
void search_value(int* data, int size, int value);
void load_data(const char* filename, int** dataset, int* size);
void save_data(const char* filename, int* dataset, int size);
void display_menu();
void process_user_input(int* dataset, int size);

#endif // MAIN_H