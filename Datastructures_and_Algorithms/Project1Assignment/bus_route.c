#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#define sleep_ms(x) Sleep(x)
#else
#include <unistd.h>
#define sleep_ms(x) usleep((x) * 1000)
#endif

// Basic configuration
#define DELAY_MS 3000
#define MAX_NAME_LEN 50

// Node structure for Doubly Linked List
typedef struct Node {
    char name[MAX_NAME_LEN];
    int number;
    struct Node* next;
    struct Node* prev;
} Node;

// Function Prototypes
Node* createNode(char* name, int number);
void appendStop(Node** head, char* name, int number);
void traverseForward(Node* head);
void traverseBackward(Node* head);
void freeList(Node* head);
void clearInputBuffer();

// Create a new node
Node* createNode(char* name, int number) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strncpy(newNode->name, name, MAX_NAME_LEN - 1);
    newNode->name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-termination
    newNode->number = number;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// Add a stop to the end of the list
void appendStop(Node** head, char* name, int number) {
    Node* newNode = createNode(name, number);

    // If list is empty, new node is head
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    // Traverse to the last node
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    // Link the new node
    temp->next = newNode;
    newNode->prev = temp;
}

// Move Forward: Home -> Campus (Head -> Tail)
void traverseForward(Node* head) {
    if (head == NULL) {
        printf("The route is empty.\n");
        return;
    }

    printf("\nStarting Forward Journey (Home -> Campus)...\n");
    Node* current = head;
    while (current != NULL) {
        printf("Stop #%d: %s\n", current->number, current->name);
        if (current->next != NULL) {
            sleep_ms(DELAY_MS); // Wait 3 seconds
        }
        current = current->next;
    }
    printf("Arrived at Campus (End of Route).\n");
}

// Move Backward: Campus -> Home (Tail -> Head)
void traverseBackward(Node* head) {
    if (head == NULL) {
        printf("The route is empty.\n");
        return;
    }

    // First, find the tail
    Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    printf("\nStarting Backward Journey (Campus -> Home)...\n");
    Node* current = temp;
    while (current != NULL) {
        printf("Stop #%d: %s\n", current->number, current->name);
        if (current->prev != NULL) {
            sleep_ms(DELAY_MS); // Wait 3 seconds
        }
        current = current->prev;
    }
    printf("Arrived at Home (Start of Route).\n");
}

// Free memory
void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    printf("Memory cleared. Exiting.\n");
}

// Helper to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Node* head = NULL;
    int choice;
    char name[MAX_NAME_LEN];
    int number;
    int initialCount;

    printf("=== Bus Route Setup ===\n");
    printf("How many stops are initially on the route? ");
    scanf("%d", &initialCount);
    clearInputBuffer();

    for (int i = 0; i < initialCount; i++) {
        printf("Enter name for Stop %d: ", i + 1);
        fgets(name, MAX_NAME_LEN, stdin);
        name[strcspn(name, "\n")] = 0; // Remove newline
        
        // Stop number could be auto-assigned or manual. Let's make it manual as per "Stop number (integer)" requirement implied inputs.
        // Actually, let's just make it i+1 or ask. The prompt says "Bus stop name" and "Stop number". Let's ask.
        printf("Enter number for Stop %d: ", i + 1);
        scanf("%d", &number);
        clearInputBuffer();
        
        appendStop(&head, name, number);
    }

    do {
        printf("\n=== Bus Navigation Menu ===\n");
        printf("1. Move Forward (Home -> Campus)\n");
        printf("2. Move Backward (Campus -> Home)\n");
        printf("3. Add New Stop\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                traverseForward(head);
                break;
            case 2:
                traverseBackward(head);
                break;
            case 3:
                printf("Enter new stop name: ");
                fgets(name, MAX_NAME_LEN, stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Enter new stop number: ");
                scanf("%d", &number);
                clearInputBuffer();
                appendStop(&head, name, number);
                printf("New stop added.\n");
                break;
            case 4:
                freeList(head);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);

    return 0;
}
