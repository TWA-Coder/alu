#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    // Process-related system call
    pid_t pid = getpid();
    printf("Process ID: %d\n", pid);

    // 1. File creation and writing logs
    printf("Attempting to create and write to file...\n");
    int fd = open("system_monitor_log.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error creating file");
        return 1;
    }

    const char *log_msg = "System Call Monitoring Log Entry: Program executed successfully.\n";
    ssize_t bytes_written = write(fd, log_msg, strlen(log_msg));
    if (bytes_written < 0) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }
    printf("Successfully wrote %zd bytes to the log file.\n", bytes_written);
    close(fd);

    // 2. Reading files
    printf("Attempting to read from the file...\n");
    fd = open("system_monitor_log.txt", O_RDONLY);
    if (fd < 0) {
        perror("Error opening file for reading");
        return 1;
    }

    char buffer[256];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Error reading from file");
        close(fd);
        return 1;
    }
    
    buffer[bytes_read] = '\0'; // Null-terminate the string
    printf("Read from file: %s", buffer);
    close(fd);

    return 0;
}
