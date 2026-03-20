#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Using a 256-byte buffer similar to the syscall version
// Stdio will buffer this into larger chunk (typically 4096 or 8192 bytes) 
// transparently before asking the OS via syscall.
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source> <dest>\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "rb");
    if (!src) {
        perror("fopen src");
        return 1;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (!dest) {
        perror("fopen dest");
        fclose(src);
        return 1;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    char buf[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, sizeof(buf), src)) > 0) {
        if (fwrite(buf, 1, bytes_read, dest) != bytes_read) {
            perror("fwrite error");
            break;
        }
    }

    gettimeofday(&end, NULL);
    fclose(src);
    fclose(dest);

    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    
    printf("Standard I/O (fread/fwrite) - Target File Copied\n");
    printf("Execution Time: %.6f seconds\n", time_taken);
    return 0;
}
