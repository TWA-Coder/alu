#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

// Structure to represent a Job
typedef struct {
    char id;
    int priority;
} Job;

// Structure to represent the Max-Heap
typedef struct {
    Job array[MAX_SIZE];
    int size;
} Heap;

// Function to swap two jobs
void swap(Job *a, Job *b) {
    Job temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the Max-Heap property
// idx: index of the node to heapify
void max_heapify(Heap *h, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // Compare with left child
    if (left < h->size && h->array[left].priority > h->array[largest].priority)
        largest = left;

    // Compare with right child
    if (right < h->size && h->array[right].priority > h->array[largest].priority)
        largest = right;

    // If largest is not root, swap and recursively heapify
    if (largest != idx) {
        swap(&h->array[idx], &h->array[largest]);
        max_heapify(h, largest);
    }
}

// Function to build a Max-Heap from an unsorted array
void build_max_heap(Heap *h) {
    // Start from the last non-leaf node and heapify down to the root
    for (int i = (h->size / 2) - 1; i >= 0; i--) {
        max_heapify(h, i);
    }
}

// Function to insert a new job into the heap
void insert_job(Heap *h, Job job) {
    if (h->size >= MAX_SIZE) {
        printf("Heap overflow! Cannot insert job.\n");
        return;
    }

    // Insert new job at the end
    int i = h->size;
    h->array[i] = job;
    h->size++;

    // Fix the Max-Heap property by bubbling up
    while (i != 0 && h->array[(i - 1) / 2].priority < h->array[i].priority) {
        swap(&h->array[i], &h->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Function to extract the job with the highest priority
Job extract_max(Heap *h) {
    Job max_job = {'\0', -1}; // Return a dummy job if heap is empty

    if (h->size <= 0) {
        printf("Heap underflow! No jobs to extract.\n");
        return max_job;
    }

    if (h->size == 1) {
        h->size--;
        return h->array[0];
    }

    // Store the max job (root)
    max_job = h->array[0];

    // Move the last job to the root
    h->array[0] = h->array[h->size - 1];
    h->size--;

    // Restore the Max-Heap property
    max_heapify(h, 0);

    return max_job;
}

// Helper function to print the heap structure
void print_heap(Heap *h, const char *message) {
    printf("\n--- %s ---\n", message);
    printf("Heap Size: %d\n", h->size);
    printf("Job Priorities (Level-order): ");
    for (int i = 0; i < h->size; i++) {
        printf("%c:%d ", h->array[i].id, h->array[i].priority);
    }
    printf("\n");
    
    // Optional: Print tree-like structure for better visualization (limited depth)
    // Root
    if (h->size > 0) printf("Root: [%c:%d]\n", h->array[0].id, h->array[0].priority);
}

int main() {
    // Initial priorities
    int priorities[] = {42, 17, 93, 28, 65, 81, 54, 60, 99, 73, 88};
    int n = sizeof(priorities) / sizeof(priorities[0]);

    Heap h;
    h.size = 0;

    // Fill heap with initial jobs
    // Assigning IDs A, B, C... sequentially
    for (int i = 0; i < n; i++) {
        Job job;
        job.id = 'A' + i;
        job.priority = priorities[i];
        h.array[i] = job;
    }
    h.size = n;

    // 1. Build Max-Heap
    printf("Initial Jobs loaded (Unsorted).\n");
    build_max_heap(&h);
    print_heap(&h, "After Building Max-Heap");

    // 2. Insert urgent job with priority 100
    Job urgent_job = {'L', 100};
    printf("\n>>> Inserting Urgent Job (ID: L, Priority: 100) <<<\n");
    insert_job(&h, urgent_job);
    print_heap(&h, "After Inserting Job 100");


    printf("\n>>> Aborting/Extracting Highest Priority Job <<<\n");
    Job max_job = extract_max(&h);
    printf("Extracted Job: %c with Priority %d\n", max_job.id, max_job.priority);
    
    if (max_job.priority == 100) {
        printf("Verified: The urgent job (Priority 100) was removed.\n");
    } else {
        printf("Note: The urgent job was NOT the one removed (Check logic).\n");
    }

    print_heap(&h, "After Extracting Max (Restoring Heap Property)");

    return 0;
}
