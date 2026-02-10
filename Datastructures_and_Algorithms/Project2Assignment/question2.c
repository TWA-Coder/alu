#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// Structure to represent a Passenger
typedef struct {
    char name[50];
    int risk_score;
} Passenger;

// Structure to represent the Max-Heap
typedef struct {
    Passenger array[MAX_SIZE];
    int size;
} Heap;

// Function to swap two passengers
void swap(Passenger *a, Passenger *b) {
    Passenger temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the Max-Heap property
void max_heapify(Heap *h, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // Compare with left child
    if (left < h->size && h->array[left].risk_score > h->array[largest].risk_score)
        largest = left;

    // Compare with right child
    if (right < h->size && h->array[right].risk_score > h->array[largest].risk_score)
        largest = right;

    // If largest is not root, swap and recursively heapify
    if (largest != idx) {
        swap(&h->array[idx], &h->array[largest]);
        max_heapify(h, largest);
    }
}

// Function to build a Max-Heap
void build_max_heap(Heap *h) {
    for (int i = (h->size / 2) - 1; i >= 0; i--) {
        max_heapify(h, i);
    }
}

// Function to insert a new passenger
void insert_passenger(Heap *h, char *name, int risk_score) {
    if (h->size >= MAX_SIZE) {
        printf("Heap full! Cannot add passenger %s.\n", name);
        return;
    }

    // Add at end
    int i = h->size;
    strcpy(h->array[i].name, name);
    h->array[i].risk_score = risk_score;
    h->size++;

    // Bubble up
    while (i != 0 && h->array[(i - 1) / 2].risk_score < h->array[i].risk_score) {
        swap(&h->array[i], &h->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    printf("Inserted: %s (Risk: %d)\n", name, risk_score);
}

// Function to delete a passenger by risk score (removes the first match found)
// Returns 1 if successful, 0 if not found
int delete_passenger(Heap *h, int risk_score) {
    int i;
    int found = -1;

    // Find element
    for (i = 0; i < h->size; i++) {
        if (h->array[i].risk_score == risk_score) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Passenger with risk score %d not found.\n", risk_score);
        return 0;
    }

    printf("Removing: %s (Risk: %d)\n", h->array[found].name, h->array[found].risk_score);

    // Replace with last element
    h->array[found] = h->array[h->size - 1];
    h->size--;

    // If the replaced node is smaller than parent, bubble up (unlikely for max heap delete usually, but needed if we disrupted order)
    // Actually, usually we just heapify down, but if we swapped a larger element up from the bottom (unlikely in max heap context for a deletion of a large element), 
    // we strictly need to check both directions or deciding based on values.
    // However, simplest general delete update:
    
    // Check if it needs to go up
    int current = found;
    while (current != 0 && h->array[(current - 1) / 2].risk_score < h->array[current].risk_score) {
        swap(&h->array[current], &h->array[(current - 1) / 2]);
        current = (current - 1) / 2;
    }
    
    // Check if it needs to go down
    max_heapify(h, current);

    return 1;
}

// Function to extract highest priority (for processing)
void extract_max(Heap *h) {
    if (h->size <= 0) return;
    printf("Processing Passenger: %s (Risk: %d)\n", h->array[0].name, h->array[0].risk_score);
    
    h->array[0] = h->array[h->size - 1];
    h->size--;
    max_heapify(h, 0);
}

// Display Priority Queue
void display_heap(Heap *h) {
    printf("\n--- Security Queue (Highest Risk First) ---\n");
    if (h->size == 0) {
        printf("Queue is empty.\n");
        return;
    }
    
    printf("Root Name: %s | Risk: %d\n", h->array[0].name, h->array[0].risk_score);
    printf("Full Queue (Level Order):\n");
    for (int i = 0; i < h->size; i++) {
        printf("[%s: %d] ", h->array[i].name, h->array[i].risk_score);
    }
    printf("\n-------------------------------------------\n");
}

int main() {
    Heap h;
    h.size = 0;

    // 1. Initialize Passengers
    // Using arbitrary risk scores as placeholders
    struct { char *name; int score; } initial_data[] = {
        {"Alice", 30},
        {"Brian", 55},
        {"Chen", 88},
        {"Fatima", 70},
        {"Noah", 45},
        {"Sofia", 60}
    };
    int n = sizeof(initial_data) / sizeof(initial_data[0]);

    for (int i = 0; i < n; i++) {
        strcpy(h.array[i].name, initial_data[i].name);
        h.array[i].risk_score = initial_data[i].score;
    }
    h.size = n;

    printf("Initializing System with %d passengers...\n", n);
    
    // 2. Build Max-Heap
    build_max_heap(&h);
    display_heap(&h);

    // 3. New Passenger arrives with Risk 98
    printf("\n>>> EVENT: New Passenger Arrives <<<\n");
    insert_passenger(&h, "NewGuy", 98);
    display_heap(&h);

    // Verify it is at root
    if (h.size > 0 && h.array[0].risk_score == 98) {
        printf("System Check: New Passenger is at the FRONT of the line.\n");
    }

    // 4. Passenger with risk 98 is cleared (removed)
    // Note: If they are at the front, we could just extract_max. 
    // But demonstrating specific deletion logic for robustness.
    printf("\n>>> EVENT: Passenger with Risk 98 is Cleared/Removed <<<\n");
    delete_passenger(&h, 98);
    display_heap(&h);

    return 0;
}
