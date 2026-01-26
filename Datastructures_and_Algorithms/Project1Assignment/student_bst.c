#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
// Assuming a pipe delimiter in file as per prompt "Firstname| Lastname| Grade" 
// Or space separated as per my plan. The prompt said "Firstname| Lastname| Grade" in the description but standard text files often vary.
// I will support the pipe format to be robust since it was explicitly mentioned.

typedef struct {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int grade;
} Student;

typedef struct Node {
    Student data;
    struct Node *left;
    struct Node *right;
} Node;

// Prototypes
Node* createNode(Student data);
Node* insert(Node* root, Student data);
void searchByLastName(Node* root, char* lastName, int* foundCount);
void freeTree(Node* root);
void readRecords(Node** root, const char* filename);

// Create a new node
Node* createNode(Student data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Insert into BST based on LastName
// Duplicate LastNames go to the RIGHT subtree
Node* insert(Node* root, Student data) {
    if (root == NULL) {
        return createNode(data);
    }

    // Compare Last Names
    int cmp = strcmp(data.lastName, root->data.lastName);
    
    if (cmp < 0) {
        // Smaller goes Left
        root->left = insert(root->left, data);
    } else {
        // Larger OR Equal (Duplicates) goes Right
        root->right = insert(root->right, data);
    }
    return root;
}

// Search and print all students with matching LastName
void searchByLastName(Node* root, char* lastName, int* foundCount) {
    if (root == NULL) {
        return;
    }

    int cmp = strcmp(lastName, root->data.lastName);

    if (cmp == 0) {
        // Match found!
        printf("Found: %s %s, Grade: %d\n", 
               root->data.firstName, root->data.lastName, root->data.grade);
        (*foundCount)++;
        
        // Since duplicates (same last name) are inserted to the RIGHT,
        // we must continue searching the right subtree to find others.
        searchByLastName(root->right, lastName, foundCount);
    } 
    else if (cmp < 0) {
        // Target is smaller, go left
        searchByLastName(root->left, lastName, foundCount);
    } 
    else { // cmp > 0
        // Target is larger, go right
        searchByLastName(root->right, lastName, foundCount);
    }
}

// Read records from file
void readRecords(Node** root, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[150];
    while (fgets(line, sizeof(line), file)) {
        Student s;
        // Try parsing assuming "First Last Grade" (space separated) first
        // If the file actually uses pipes "First| Last| Grade", we need to handle that.
        // Let's implement robust parsing.
        
        char *token;
        char tempLine[150];
        strcpy(tempLine, line);
        
        // Remove checking for pipe vs space, just use format string that handles simpler space-sep first as per typical C assignments
        // unless strictly pipe specific. Let's try basic fscanf-style parsing from the line string.
        
        // Determine format: Check for pipe
        if (strchr(line, '|')) {
            // Pipe format: "First| Last| Grade"
             if (sscanf(line, "%[^|]| %[^|]| %d", s.firstName, s.lastName, &s.grade) == 3) {
                 // Trim whitespace if needed? sscanf %[^|] reads up to pipe.
                 // Clean up potential trailing spaces isn't automatic with %[^|], but usually okay for this level.
             } else {
                 continue; // Skip malformed
             }
        } else {
            // Standard space format: "First Last Grade"
            if (sscanf(line, "%s %s %d", s.firstName, s.lastName, &s.grade) != 3) {
                continue; 
            }
        }
        
        *root = insert(*root, s);
    }
    fclose(file);
}

void freeTree(Node* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    Node* root = NULL;
    char filename[] = "student_records.txt";
    char searchName[MAX_NAME_LEN];
    int choice;

    printf("Reading student records from %s...\n", filename);
    readRecords(&root, filename);
    printf("BST Construction Complete.\n");

    do {
        printf("\n=== Student Record System (BST) ===\n");
        printf("1. Search by Last Name\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
             while(getchar() != '\n');
             choice = 0;
        }

        if (choice == 1) {
            printf("Enter Last Name to search: ");
            scanf("%s", searchName);
            
            int count = 0;
            printf("\n--- Search Results ---\n");
            searchByLastName(root, searchName, &count);
            if (count == 0) {
                printf("No records found for '%s'.\n", searchName);
            } else {
                printf("Total records found: %d\n", count);
            }
            printf("----------------------\n");
        }

    } while (choice != 2);

    freeTree(root);
    return 0;
}
