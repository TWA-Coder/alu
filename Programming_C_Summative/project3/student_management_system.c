#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_COURSE_LENGTH 50
#define MAX_GRADES 10

// Structure to store student details
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    char course[MAX_COURSE_LENGTH];
    float grades[MAX_GRADES];
    int numGrades;
    float gpa;
} Student;

// Function prototypes
void addStudent(Student **students, int *size);
void displayStudents(Student *students, int size);
void updateStudent(Student *students, int size);
void deleteStudent(Student **students, int *size);
void saveToFile(Student *students, int size);
void loadFromFile(Student **students, int *size);
void searchStudent(Student *students, int size);
void sortStudents(Student *students, int size);
void generateReports(Student *students, int size);
float computeAverageGPA(Student *students, int size);
void freeMemory(Student *students);

int main() {
    Student *students = NULL;
    int size = 0;
    int choice;

    loadFromFile(&students, &size);

    do {
        printf("\nStudent Management System\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Save to File\n");
        printf("6. Load from File\n");
        printf("7. Search Student\n");
        printf("8. Sort Students\n");
        printf("9. Generate Reports\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(&students, &size); break;
            case 2: displayStudents(students, size); break;
            case 3: updateStudent(students, size); break;
            case 4: deleteStudent(&students, &size); break;
            case 5: saveToFile(students, size); break;
            case 6: loadFromFile(&students, &size); break;
            case 7: searchStudent(students, size); break;
            case 8: sortStudents(students, size); break;
            case 9: generateReports(students, size); break;
            case 0: freeMemory(students); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}

// Function to add a student
void addStudent(Student **students, int *size) {
    *students = realloc(*students, (*size + 1) * sizeof(Student));
    if (*students == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    Student *newStudent = &(*students)[*size];
    printf("Enter ID: ");
    scanf("%d", &newStudent->id);
    printf("Enter Name: ");
    scanf(" %[^\n]", newStudent->name);
    printf("Enter Age: ");
    scanf("%d", &newStudent->age);
    printf("Enter Course: ");
    scanf(" %[^\n]", newStudent->course);
    printf("Enter number of grades: ");
    scanf("%d", &newStudent->numGrades);
    printf("Enter Grades: ");
    for (int i = 0; i < newStudent->numGrades; ++i) {
        scanf("%f", &newStudent->grades[i]);
    }

    // Calculate GPA
    float total = 0;
    for (int i = 0; i < newStudent->numGrades; ++i) {
        total += newStudent->grades[i];
    }
    newStudent->gpa = total / newStudent->numGrades;
    (*size)++;
}

// Function to display all students
void displayStudents(Student *students, int size) {
    printf("\nStudent Records:\n");
    for (int i = 0; i < size; i++) {
        printf("ID: %d, Name: %s, Age: %d, Course: %s, GPA: %.2f\n",
               students[i].id, students[i].name, students[i].age, students[i].course, students[i].gpa);
    }
}

// Function to update a student's record
void updateStudent(Student *students, int size) {
    int id;
    printf("Enter ID of student to update: ");
    scanf("%d", &id);

    for (int i = 0; i < size; i++) {
        if (students[i].id == id) {
            printf("Updating record for ID: %d\n", id);
            printf("Enter new Name: ");
            scanf(" %[^\n]", students[i].name);
            printf("Enter new Age: ");
            scanf("%d", &students[i].age);
            printf("Enter new Course: ");
            scanf(" %[^\n]", students[i].course);
            printf("Enter new number of grades: ");
            scanf("%d", &students[i].numGrades);
            printf("Enter new Grades: ");
            for (int j = 0; j < students[i].numGrades; ++j) {
                scanf("%f", &students[i].grades[j]);
            }

            // Recalculate GPA
            float total = 0;
            for (int j = 0; j < students[i].numGrades; ++j) {
                total += students[i].grades[j];
            }
            students[i].gpa = total / students[i].numGrades;
            printf("Record updated successfully!\n");
            return;
        }
    }
    printf("Student with ID %d not found!\n", id);
}

// Function to delete a student record
void deleteStudent(Student **students, int *size) {
    int id;
    printf("Enter ID of student to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *size; i++) {
        if ((*students)[i].id == id) {
            for (int j = i; j < *size - 1; j++) {
                (*students)[j] = (*students)[j + 1]; // Shift records left
            }
            *students = realloc(*students, (*size - 1) * sizeof(Student));
            (*size)--;
            printf("Record deleted successfully!\n");
            return;
        }
    }
    printf("Student with ID %d not found!\n", id);
}

// Function to save students to a file
void saveToFile(Student *students, int size) {
    FILE *file = fopen("students.dat", "wb");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }
    fwrite(students, sizeof(Student), size, file);
    fclose(file);
    printf("Data saved successfully!\n");
}

// Function to load students from a file
void loadFromFile(Student **students, int *size) {
    FILE *file = fopen("students.dat", "rb");
    if (!file) {
        printf("No existing data found. Starting fresh.\n");
        return;
    }
    *students = malloc(0);
    *size = 0;
    Student temp;

    while (fread(&temp, sizeof(Student), 1, file)) {
        *students = realloc(*students, (*size + 1) * sizeof(Student));
        (*students)[*size] = temp;
        (*size)++;
    }
    fclose(file);
    printf("Data loaded successfully!\n");
}

// Function to search for a student by ID or name
void searchStudent(Student *students, int size) {
    int choice;
    printf("Search by:\n1. ID\n2. Name\nEnter your choice: ");
    scanf("%d", &choice);
    if (choice == 1) {
        int id;
        printf("Enter Student ID: ");
        scanf("%d", &id);
        for (int i = 0; i < size; i++) {
            if (students[i].id == id) {
                printf("Found Student -> ID: %d, Name: %s, Age: %d, Course: %s, GPA: %.2f\n",
                       students[i].id, students[i].name, students[i].age, students[i].course, students[i].gpa);
                return;
            }
        }
        printf("Student with ID %d not found!\n", id);
    } else if (choice == 2) {
        char name[MAX_NAME_LENGTH];
        printf("Enter Student Name: ");
        scanf(" %[^\n]", name);
        for (int i = 0; i < size; i++) {
            if (strcmp(students[i].name, name) == 0) {
                printf("Found Student -> ID: %d, Name: %s, Age: %d, Course: %s, GPA: %.2f\n",
                       students[i].id, students[i].name, students[i].age, students[i].course, students[i].gpa);
                return;
            }
        }
        printf("Student with name %s not found!\n", name);
    } else {
        printf("Invalid choice!\n");
    }
}

// Function to sort students by GPA (simple implementation of Bubble Sort)
void sortStudents(Student *students, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (students[j].gpa < students[j + 1].gpa) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    printf("Students sorted by GPA successfully!\n");
}

// Function to generate reports
void generateReports(Student *students, int size) {
    if (size == 0) {
        printf("No student records available for reporting.\n");
        return;
    }

    printf("Class Average GPA: %.2f\n", computeAverageGPA(students, size));
    // Additional report generation can be added here
}

// Function to compute average GPA of the class
float computeAverageGPA(Student *students, int size) {
    float totalGPA = 0.0;
    for (int i = 0; i < size; i++) {
        totalGPA += students[i].gpa;
    }
    return totalGPA / size;
}

// Function to free allocated memory
void freeMemory(Student *students) {
    free(students);
    printf("Memory freed successfully!\n");
}