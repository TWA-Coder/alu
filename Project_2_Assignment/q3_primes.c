#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_NUM 200000
#define NUM_THREADS 16

long total_primes = 0;
pthread_mutex_t mutex;

typedef struct {
    int start;
    int end;
} ThreadData;

// Efficient function to check if a number is prime
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Thread routine to evaluate a range of numbers
void* count_primes(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    long local_count = 0;

    for (int i = data->start; i <= data->end; i++) {
        if (is_prime(i)) {
            local_count++;
        }
    }

    // Synchronize thread-local findings into the global shared counter
    pthread_mutex_lock(&mutex);
    total_primes += local_count;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    // Initialize global mutex for the shared counter
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    // Calculate segment bounds
    int range = MAX_NUM / NUM_THREADS;
    int remainder = MAX_NUM % NUM_THREADS;
    int current_start = 1;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = current_start;
        // Last thread handles any overflow remainder
        thread_data[i].end = current_start + range - 1 + (i == NUM_THREADS - 1 ? remainder : 0);
        current_start = thread_data[i].end + 1;

        if (pthread_create(&threads[i], NULL, count_primes, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Join all threads back to the main process
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutex post-processing
    pthread_mutex_destroy(&mutex);

    // Print the strictly formatted expected output
    printf("The synchronized total number of prime numbers between 1 and %d is %ld\n", MAX_NUM, total_primes);

    return 0;
}
