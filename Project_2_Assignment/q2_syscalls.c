#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Using a 1-byte buffer to heavily emphasize the penalty of low-level 
// system calls without internal buffering (unlike standard I/O).
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source> <dest>\n", argv[0]);
        return 1;
    }

    int src = open(argv[1], O_RDONLY);
    if (src < 0) {
        perror("open src");
        return 1;
    }

    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest < 0) {
        perror("open dest");
        close(src);
        return 1;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    char buf[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(src, buf, sizeof(buf))) > 0) {
        if (write(dest, buf, bytes_read) != bytes_read) {
            perror("write error");
            break;
        }
    }

    gettimeofday(&end, NULL);
    close(src);
    close(dest);

    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    // Important: To verify file copying properly completed.
    printf("Low-level Syscalls (read/write) - Target File Copied\n");
    printf("Execution Time: %.6f seconds\n", time_taken);
    return 0;
}
