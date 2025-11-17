#include "main.h"


// Dynamic memory management functions
int* create_dataset(int size) {
    return (int*)malloc(size * sizeof(int));
}

void free_dataset(int* data) {
    free(data);
}

// Mathematical and data processing functions
void compute_sum(int* data, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    printf("Sum: %d\n", sum);
}

void compute_average(int* data, int size) {
    if (size == 0) {
        printf("No data to compute average.\n");
        return;
    }
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    printf("Average: %.2f\n", (double)sum / size);
}

void find_max_min(int* data, int size) {
    if (size == 0) {
        printf("No data available.\n");
        return;
    }
    int max = INT_MIN;
    int min = INT_MAX;

    for (int i = 0; i < size; i++) {
        if (data[i] > max) max = data[i];
        if (data[i] < min) min = data[i];
    }

    printf("Max: %d, Min: %d\n", max, min);
}

void sort(int* data, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
    printf("Data sorted.\n");
}

void search_value(int* data, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (data[i] == value) {
            printf("Value %d found at index %d.\n", value, i);
            return;
        }
    }
    printf("Value %d not found.\n", value);
}

// File I/O functions
void load_data(const char* filename, int** dataset, int* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file for reading.\n");
        return;
    }

    fscanf(file, "%d", size);
    *dataset = create_dataset(*size);

    for (int i = 0; i < *size; i++) {
        fscanf(file, "%d", &(*dataset)[i]);
    }

    fclose(file);
}

void save_data(const char* filename, int* dataset, int size) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Could not open file for writing.\n");
        return;
    }

    fprintf(file, "%d\n", size);

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", dataset[i]);
    }

    fclose(file);
}

// Menu and user input processing functions
void display_menu() {
    printf("Menu:\n");
    printf("1. Compute Sum\n");
    printf("2. Compute Average\n");
    printf("3. Find Max and Min\n");
    printf("4. Sort Data\n");
    printf("5. Search Value\n");
    printf("6. Exit\n");
}

void process_user_input(int* dataset, int size) {
    int choice, value;

    while (1) {
        display_menu();
        printf("Choose an operation: ");
        scanf("%d", &choice);

        if (choice == 6) break;

        if (choice < 1 || choice > 6) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        switch (choice) {
            case 1: compute_sum(dataset, size); break;
            case 2: compute_average(dataset, size); break;
            case 3: find_max_min(dataset, size); break;
            case 4: sort(dataset, size); break;
            case 5:
                printf("Enter value to search: ");
                scanf("%d", &value);
                search_value(dataset, size, value);
                break;
        }
    }
}
