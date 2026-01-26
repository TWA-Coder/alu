#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 1000
#define MAX_NAME_LEN 50
#define INPUT_FILE "students.txt"
#define OUTPUT_FILE "sorted_students.txt"

// Student structure
typedef struct {
    int id;
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int grade;
} Student;

// Function prototypes
void readFile(Student students[], int *count);
void writeFile(Student students[], int count);
void quickSort(Student students[], int low, int high);
int partition(Student students[], int low, int high);
int compareStudents(const void *a, const void *b); // Helper for qsort logic if needed, but we implement manual quicksort

// Read records from file
void readFile(Student students[], int *count) {
    FILE *file = fopen(INPUT_FILE, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", INPUT_FILE);
        exit(1);
    }

    *count = 0;
    while (fscanf(file, "%d %s %s %d", &students[*count].id, 
                  students[*count].firstName, 
                  students[*count].lastName, 
                  &students[*count].grade) == 4) {
        (*count)++;
        if (*count >= MAX_STUDENTS) {
            printf("Warning: Maximum student limit reached.\n");
            break;
        }
    }
    fclose(file);
}

// Write sorted records to file
void writeFile(Student students[], int count) {
    FILE *file = fopen(OUTPUT_FILE, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", OUTPUT_FILE);
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %s %d\n", students[i].id, 
                students[i].firstName, 
                students[i].lastName, 
                students[i].grade);
    }
    fclose(file);
    printf("Successfully wrote %d records to %s\n", count, OUTPUT_FILE);
}

// Compare two students: 
// 1. Grade (Ascending)
// 2. FirstName (Ascending/Lexicographical) if grades are equal
int compareStudentsData(Student a, Student b) {
    if (a.grade != b.grade) {
        return a.grade - b.grade; // Ascending grade
    }
    return strcmp(a.firstName, b.firstName); // Lexicographical firstName
}

// Partition function for Quick Sort
int partition(Student students[], int low, int high) {
    Student pivot = students[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        // Condition: if students[j] <= pivot
        if (compareStudentsData(students[j], pivot) <= 0) {
            i++;
            Student temp = students[i];
            students[i] = students[j];
            students[j] = temp;
        }
    }
    Student temp = students[i + 1];
    students[i + 1] = students[high];
    students[high] = temp;
    return (i + 1);
}

// Quick Sort implementation
void quickSort(Student students[], int low, int high) {
    if (low < high) {
        int pi = partition(students, low, high);
        quickSort(students, low, pi - 1);
        quickSort(students, pi + 1, high);
    }
}

int main() {
    Student students[MAX_STUDENTS];
    int count = 0;

    printf("Reading student records...\n");
    readFile(students, &count);
    printf("Read %d records.\n", count);

    if (count > 0) {
        printf("Sorting records...\n");
        quickSort(students, 0, count - 1);
        
        printf("Writing sorted records...\n");
        writeFile(students, count);
    } else {
        printf("No records found to sort.\n");
    }

    return 0;
}
