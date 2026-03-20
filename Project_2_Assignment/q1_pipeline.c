#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int pipe1[2]; // Pipe between child 1 (ps aux) and child 2 (grep root)
    int pipe2[2]; // Pipe between child 2 (grep root) and parent

    // Create pipes
    if (pipe(pipe1) < 0) {
        perror("pipe1 error");
        exit(1);
    }
    if (pipe(pipe2) < 0) {
        perror("pipe2 error");
        exit(1);
    }

    // Fork child 1
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork1 error");
        exit(1);
    }

    if (pid1 == 0) {
        // Child 1: ps aux
        dup2(pipe1[1], STDOUT_FILENO); // Write standard output to pipe1 write end
        
        // Close unused pipes
        close(pipe1[0]);  // Child 1 doesn't read from pipe1
        close(pipe1[1]);  // Closed since we duplicated it
        close(pipe2[0]);  // Child 1 doesn't use pipe2
        close(pipe2[1]);

        char *args[] = {"ps", "aux", NULL};
        execvp(args[0], args);
        perror("execvp ps failed");
        exit(1);
    }

    // Fork child 2
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork2 error");
        exit(1);
    }

    if (pid2 == 0) {
        // Child 2: grep root
        dup2(pipe1[0], STDIN_FILENO);  // Read standard input from pipe1 read end
        dup2(pipe2[1], STDOUT_FILENO); // Write standard output to pipe2 write end
        
        // Close unused pipes
        close(pipe1[0]); // Closed since we duplicated it
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]); // Closed since we duplicated it

        char *args[] = {"grep", "root", NULL};
        execvp(args[0], args);
        perror("execvp grep failed");
        exit(1);
    }

    // Parent Process
    // Close unused pipes (important to close write ends so read() doesn't block forever)
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[1]); // Close write end of pipe2

    // Parent reads from pipe2 and writes to a file
    int file_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd < 0) {
        perror("open output.txt failed");
        exit(1);
    }

    char buffer[4096];
    ssize_t bytes_read;
    // read() will block until data is available, and return 0 when the write end is closed
    while ((bytes_read = read(pipe2[0], buffer, sizeof(buffer))) > 0) {
        write(file_fd, buffer, bytes_read);
    }

    close(pipe2[0]);
    close(file_fd);

    // Wait for both children to terminate
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    // Parent reads and displays part of the output (first 5 lines)
    printf("--- Output captured in 'output.txt' ---\n\n");
    printf("Displaying the first 5 lines of the captured output:\n");
    printf("----------------------------------------------------\n");
    
    FILE *fp = fopen("output.txt", "r");
    if (fp == NULL) {
        perror("fopen output.txt failed");
        exit(1);
    }
    
    char line[1024];
    int count = 0;
    while (fgets(line, sizeof(line), fp) != NULL && count < 5) {
        printf("%s", line);
        count++;
    }
    fclose(fp);

    printf("----------------------------------------------------\n");
    printf("Pipeline execution completed successfully.\n");
    return 0;
}
