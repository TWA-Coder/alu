#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} MinHeap;

// Function to swap elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify-up (after insertion)
void heapifyUp(MinHeap *heap, int index) {
    int parent = (index - 1) / 2;

    if (index && heap->data[parent] > heap->data[index]) {
        swap(&heap->data[parent], &heap->data[index]);
        heapifyUp(heap, parent);
    }
}

// Heapify-down (after deletion)
void heapifyDown(MinHeap *heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;

    if (left < heap->size && heap->data[left] < heap->data[smallest])
        smallest = left;

    if (right < heap->size && heap->data[right] < heap->data[smallest])
        smallest = right;

    if (smallest != index) {
        swap(&heap->data[index], &heap->data[smallest]);
        heapifyDown(heap, smallest);
    }
}

// Insert a new element into the heap
void insert(MinHeap *heap, int value) {
    if (heap->size == MAX_SIZE) {
        printf("Heap is full!\n");
        return;
    }

    heap->data[heap->size] = value;
    heapifyUp(heap, heap->size);
    heap->size++;
}

// Remove and return the minimum element
int deleteMin(MinHeap *heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        return -1;
    }

    int root = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapifyDown(heap, 0);

    return root;
}

// Print the heap elements
void printHeap(MinHeap *heap) {
    printf("Heap: ");
    for (int i = 0; i < heap->size; i++)
        printf("%d ", heap->data[i]);
    printf("\n");
}

// Main function to demonstrate
int main() {
    MinHeap heap;
    heap.size = 0;

    insert(&heap, 15);
    insert(&heap, 10);
    insert(&heap, 20);
    insert(&heap, 8);
    insert(&heap, 25);

    printHeap(&heap);

    //printf("Deleted min: %d\n", deleteMin(&heap));
    //printHeap(&heap);

    //printf("Deleted min: %d\n", deleteMin(&heap));
    //printHeap(&heap);

    return 0;
}
