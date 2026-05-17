#include "attendance_system.h"

Student registry[MAX_STUDENTS];
int num_students = 0;

int load_registry(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("ERROR: Could not open %s or it is missing.\n", filename);
        return 0;
    }

    char line[256];
    num_students = 0;

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = 0;
        
        if (strlen(line) == 0) continue;

        if (num_students >= MAX_STUDENTS) {
            printf("WARNING: Max students reached.\n");
            break;
        }

        // Format: student_id,full_name,course_code
        char* id = strtok(line, ",");
        char* name = strtok(NULL, ",");
        char* course = strtok(NULL, ",");

        if (id && name && course) {
            strncpy(registry[num_students].student_id, id, sizeof(registry[num_students].student_id) - 1);
            strncpy(registry[num_students].full_name, name, sizeof(registry[num_students].full_name) - 1);
            strncpy(registry[num_students].course_code, course, sizeof(registry[num_students].course_code) - 1);
            
            // Ensure null termination
            registry[num_students].student_id[19] = '\0';
            registry[num_students].full_name[49] = '\0';
            registry[num_students].course_code[9] = '\0';
            
            num_students++;
        }
    }

    fclose(file);
    if (num_students == 0) {
        printf("ERROR: %s is empty or invalid.\n", filename);
        return 0;
    }
    return 1;
}

Student* find_student(const char* student_id) {
    for (int i = 0; i < num_students; i++) {
        if (strcmp(registry[i].student_id, student_id) == 0) {
            return &registry[i];
        }
    }
    return NULL;
}
