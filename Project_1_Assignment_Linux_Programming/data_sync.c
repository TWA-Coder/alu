#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void sync_files(const char *source, const char *destination) {
    printf("Synchronizing files from %s to %s...\n", source, destination);
    sleep(1); // Simulate time taken for synchronization
    printf("Synchronization complete.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_dir> <destination_dir>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    printf("Starting data_sync utility...\n");
    sync_files(argv[1], argv[2]);
    
    return EXIT_SUCCESS;
}
