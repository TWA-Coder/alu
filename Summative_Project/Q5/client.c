#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int sock;
    struct sockaddr_in server_args;
    char buffer[BUFFER_SIZE];
    char user_id[20];

    // TCP Socket Creation
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not instantiate local TCP boundaries.");
        return 1;
    }

    server_args.sin_family = AF_INET;
    server_args.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect Locally
    server_args.sin_port = htons(PORT);

    // Initial Connection Attempt
    if (connect(sock, (struct sockaddr *)&server_args, sizeof(server_args)) < 0) {
        perror("Initial connection bounding failed severely.");
        return 1;
    }

    printf("=========================================\n");
    printf(" Welcome to the Digital Library Terminal \n");
    printf("=========================================\n");
    
    // -----------------------------------------------------
    // 1. Authentication Phase
    // -----------------------------------------------------
    printf("Please enter your designated Library ID: ");
    scanf("%19s", user_id);

    // Draft Protocol Message: AUTH:ALU123
    char auth_msg[50];
    sprintf(auth_msg, "AUTH:%s", user_id);
    
    if (send(sock, auth_msg, strlen(auth_msg), 0) < 0) {
        puts("Transmission of Auth Payload completely failed.");
        return 1;
    }

    memset(buffer, 0, BUFFER_SIZE);
    if (recv(sock, buffer, BUFFER_SIZE, 0) < 0) {
        puts("Recv Failed.");
        return 1;
    }

    if (strncmp(buffer, "AUTH_FAIL", 9) == 0) {
        printf("\n[ERROR] Authentication actively rejected by central server!\n");
        printf("Invalid credentials matching. Terminating connection.\n");
        close(sock);
        return 0;
    }

    printf("\n[SUCCESS] Server authenticated credentials!\n\n");

    // -----------------------------------------------------
    // 2. Fetch Catalog Array Payload
    // -----------------------------------------------------
    if (send(sock, "GET_BOOKS", 9, 0) < 0) {
        puts("Transmission payload failed.");
        return 1;
    }

    memset(buffer, 0, BUFFER_SIZE);
    printf("Fetching mathematical catalogs...\n\n");
    
    // Read the chunked streams until we receive the standard termination flag
    while(recv(sock, buffer, BUFFER_SIZE - 1, 0) > 0) {
        // Did we hit the custom EOF EOF block?
        char *end_ptr = strstr(buffer, "END_BOOKS");
        if (end_ptr != NULL) {
            *end_ptr = '\0'; // Snip it clean so it doesn't print garbled data bounds
            printf("%s", buffer);
            break;
        }
        printf("%s", buffer);
        memset(buffer, 0, BUFFER_SIZE); // Re-clear for next internal stream
    }

    // -----------------------------------------------------
    // 3. User Selection Input
    // -----------------------------------------------------
    char book_target[10];
    printf("\nSelect internal book ID you wish to secure reservation upon: ");
    scanf("%9s", book_target);

    // Construct standard protocol message: RESERVE:1
    char res_msg[50];
    sprintf(res_msg, "RESERVE:%s", book_target);
    send(sock, res_msg, strlen(res_msg), 0);

    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE, 0);

    // Interpret Standard Responses
    if (strncmp(buffer, "RES_SUCCESS", 11) == 0) {
        printf("\n[+] Reservation processed securely by server mutex constraints!\n");
    } else if (strncmp(buffer, "RES_REJECTED", 12) == 0) {
        printf("\n[-] RESERVATION REFUSED. Book is heavily locked/already reserved by another physical entity.\n");
    } else {
        printf("\n[-] Invalid Book ID parameters passed into memory boundaries.\n");
    }

    // -----------------------------------------------------
    // 4. Session Tear-down
    // -----------------------------------------------------
    printf("\nSession closed. Goodbye, %s\n", user_id);
    close(sock);
    
    return 0;
}
