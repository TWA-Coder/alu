#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Signal handler function
void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\n\nMonitor Service shutting down safely.\n");
        exit(0);
    } else if (sig == SIGUSR1) {
        printf("\nSystem status requested by administrator.\n");
    } else if (sig == SIGTERM) {
        printf("\nEmergency shutdown signal received.\n");
        exit(1);
    }
}

int main() {
    // 1. Register signal handlers
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = SA_RESTART; // Restart functions if interrupted by handler
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error registering SIGINT handler");
        exit(1);
    }

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error registering SIGUSR1 handler");
        exit(1);
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Error registering SIGTERM handler");
        exit(1);
    }

    printf("Starting Monitor Service (PID: %d)...\n", getpid());
    printf("Waiting for signals. Try sending SIGINT (Ctrl+C), SIGUSR1, or SIGTERM.\n\n");

    // 2. Infinite monitoring loop
    while (1) {
        printf("[Monitor Service] System running normally...\n");
        sleep(5);
    }

    return 0;
}
