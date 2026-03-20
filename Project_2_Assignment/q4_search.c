#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

// Shared Task Variables
char* keyword;
FILE* output_fp;
char** file_list;
int total_files;

// Synchronization Primitives
int current_file_idx = 0;
pthread_mutex_t idx_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t out_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to read entire file and count exact keyword occurrences
int count_occurrences(const char* filename, const char* target) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    // Use fseek to get file size for exact heap allocation
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (size == 0) {
        fclose(fp);
        return 0;
    }

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(fp);
        return -1;
    }

    // Standard I/O block read
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);

    int count = 0;
    size_t target_len = strlen(target);
    char *ptr = buffer;
    
    // Find all occurrences within the buffer
    while ((ptr = strstr(ptr, target)) != NULL) {
        count++;
        ptr += target_len; // Move pointer forward to avoid counting overlapping pieces
    }

    free(buffer);
    return count;
}

// Thread routine representing our worker pool
void* worker_thread(void* arg) {
    while (1) {
        // Retrieve the next file index safely
        int idx;
        pthread_mutex_lock(&idx_mutex);
        if (current_file_idx >= total_files) {
            pthread_mutex_unlock(&idx_mutex);
            break; // No more files to process in the queue
        }
        idx = current_file_idx++;
        pthread_mutex_unlock(&idx_mutex);

        const char* filename = file_list[idx];
        int count = count_occurrences(filename, keyword);

        // Safely write results back to the shared output file
        pthread_mutex_lock(&out_mutex);
        if (count >= 0) {
            fprintf(output_fp, "File: %s | Occurrences of '%s': %d\n", filename, keyword, count);
        } else {
            fprintf(output_fp, "File: %s | Error: Failed to open or read file\n", filename);
        }
        pthread_mutex_unlock(&out_mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    // Requires keyword, output path, at least 1 file, and thread count
    if (argc < 5) {
        printf("Usage format: ./search <keyword> <output.txt> <file1.txt> <file2.txt> ... <number_of_threads>\n");
        return 1;
    }

    keyword = argv[1];
    char* output_filename = argv[2];
    
    int num_threads = atoi(argv[argc - 1]);
    if (num_threads <= 0) {
        printf("Error: <number_of_threads> must be a positive integer.\n");
        return 1;
    }

    total_files = argc - 4;
    file_list = &argv[3];

    output_fp = fopen(output_filename, "w");
    if (!output_fp) {
        perror("Failed to open output file");
        return 1;
    }

    // Restrict idle threads if user inputs more threads than files
    if (num_threads > total_files) {
        num_threads = total_files;
    }

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    if (!threads) {
        perror("malloc failed for threads");
        fclose(output_fp);
        return 1;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Launch worker pool
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, worker_thread, NULL) != 0) {
            perror("Failed to create thread");
        }
    }

    // Wait for all threads representing the pool to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    fclose(output_fp);
    free(threads);

    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("[*] Processed %d files using %d threads simultaneously.\n", total_files, num_threads);
    printf("[*] Execution Time: %.6f seconds\n", time_taken);
    printf("[*] Synchronized writes saved out to %s\n\n", output_filename);

    return 0;
}
