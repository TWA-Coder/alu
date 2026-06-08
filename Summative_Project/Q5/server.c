#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define TOTAL_BOOKS 5
#define TOTAL_USERS 3

/* =====================================================================
 * DATA STRUCTURES & SHARED RESOURCES
 * ===================================================================== */
typedef struct {
    int book_id;
    char title[50];
    int is_reserved; // 0 = Available, 1 = Reserved
} Book;

// Pre-defined Catalog
Book catalog[TOTAL_BOOKS] = {
    {1, "The C Programming Language", 0},
    {2, "Operating System Concepts", 0},
    {3, "Advanced Programming in the UNIX Env", 0},
    {4, "Computer Networking: A Top-Down Approach", 0},
    {5, "Clean Code", 0}
};

// Pre-defined Auth Users
char valid_users[TOTAL_USERS][20] = { "ALU123", "ALU456", "ALU789" };
int active_users_count = 0;

// Synchronization Mechanism
// Protects BOTH the catalog state (preventing race condition reservations)
// and the active network accounting bounds.
pthread_mutex_t lib_mutex = PTHREAD_MUTEX_INITIALIZER;

/* =====================================================================
 * HELPER FUNCTIONS
 * ===================================================================== */
int is_authenticated(char *input_id) {
    for (int i = 0; i < TOTAL_USERS; i++) {
        if (strcmp(valid_users[i], input_id) == 0) {
            return 1;
        }
    }
    return 0;
}

/* =====================================================================
 * CLIENT SESSION LIFECYCLE (CONCURRENCY THREAD ROUTINE)
 * ===================================================================== */
void *handle_client(void *socket_desc) {
    int client_sock = *(int*)socket_desc;
    char buffer[BUFFER_SIZE];
    char user_id[20] = "";
    int authenticated = 0;

    // 1. Authentication Phase
    memset(buffer, 0, BUFFER_SIZE);
    if (recv(client_sock, buffer, BUFFER_SIZE, 0) > 0) {
        // Expected protocol message: "AUTH:ALU123"
        if (strncmp(buffer, "AUTH:", 5) == 0) {
            strcpy(user_id, buffer + 5);
            // Clean potentially captured newlines depending on client IO
            user_id[strcspn(user_id, "\r\n")] = '\0'; 
            
            if (is_authenticated(user_id)) {
                authenticated = 1;
                send(client_sock, "AUTH_SUCCESS\n", 13, 0);
                
                pthread_mutex_lock(&lib_mutex);
                active_users_count++;
                printf("[SERVER] User %s connected successfully. Total Active: %d\n", user_id, active_users_count);
                pthread_mutex_unlock(&lib_mutex);
            } else {
                send(client_sock, "AUTH_FAIL\n", 10, 0);
                close(client_sock);
                free(socket_desc);
                return NULL;
            }
        }
    }

    // Attempt to parse requests only if successfully authenticated
    while (authenticated) {
        memset(buffer, 0, BUFFER_SIZE);
        int read_size = recv(client_sock, buffer, BUFFER_SIZE, 0);
        
        if (read_size <= 0) {
            printf("[SERVER] Client %s disconnected unexpectedly.\n", user_id);
            break; // Connection dropped or "QUIT" triggered EOF
        }

        buffer[strcspn(buffer, "\r\n")] = '\0'; 

        // 2. Client requests "GET_BOOKS"
        if (strcmp(buffer, "GET_BOOKS") == 0) {
            char book_list[BUFFER_SIZE] = "ID | Title | Status\n-----------------------\n";
            char temp[100];
            
            pthread_mutex_lock(&lib_mutex);
            for (int i = 0; i < TOTAL_BOOKS; i++) {
                sprintf(temp, "%d | %s | %s\n", 
                        catalog[i].book_id, 
                        catalog[i].title, 
                        catalog[i].is_reserved ? "RESERVED" : "AVAILABLE");
                strcat(book_list, temp);
            }
            pthread_mutex_unlock(&lib_mutex);
            
            // Add custom termination marker to make sure client buffers properly
            strcat(book_list, "END_BOOKS"); 
            send(client_sock, book_list, strlen(book_list), 0);
        }
        
        // 3. Client requests "RESERVE:<id>"
        else if (strncmp(buffer, "RESERVE:", 8) == 0) {
            int wanted_id = atoi(buffer + 8);
            int valid_id = 0;
            
            pthread_mutex_lock(&lib_mutex);
            for (int i = 0; i < TOTAL_BOOKS; i++) {
                if (catalog[i].book_id == wanted_id) {
                    valid_id = 1;
                    if (catalog[i].is_reserved == 0) {
                        catalog[i].is_reserved = 1; // Perform the Reservation securely
                        send(client_sock, "RES_SUCCESS\n", 12, 0);
                        printf("[SERVER] User %s freshly reserved: '%s'.\n", user_id, catalog[i].title);
                    } else {
                        send(client_sock, "RES_REJECTED\n", 13, 0);
                        printf("[SERVER] User %s attempted to snake '%s', but already reserved.\n", user_id, catalog[i].title);
                    }
                    break;
                }
            }
            pthread_mutex_unlock(&lib_mutex);
            
            if (!valid_id) {
                // Given ID didn't match any index bounds. 
                send(client_sock, "RES_INVALID\n", 12, 0);
            }
        }
    }

    // 4. Session Teardown
    pthread_mutex_lock(&lib_mutex);
    active_users_count--;
    printf("[SERVER] Session closed for user %s. Active Users roughly: %d\n", user_id, active_users_count);
    pthread_mutex_unlock(&lib_mutex);

    close(client_sock);
    free(socket_desc);
    return NULL;
}

/* =====================================================================
 * SERVER APPLICATION ENTRY
 * ===================================================================== */
int main(void) {
    int server_sock, client_sock, c;
    struct sockaddr_in server_args, client_args;

    // TCP Socket Creation
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        printf("Could not instantiate isolated structural socket.");
        return 1;
    }

    // Fix "Address already in use" if restarting quickly
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_args.sin_family = AF_INET;
    server_args.sin_addr.s_addr = INADDR_ANY;
    server_args.sin_port = htons(PORT);

    // Bind Port execution space
    if (bind(server_sock, (struct sockaddr *)&server_args, sizeof(server_args)) < 0) {
        perror("Binding mathematical framework strictly faltered.");
        return 1;
    }

    listen(server_sock, MAX_CLIENTS);
    printf("=========================================\n");
    printf(" Digital Library Internal Server Active  \n");
    printf(" Listening strictly on TCP config: %d \n", PORT);
    printf("=========================================\n");

    c = sizeof(struct sockaddr_in);

    // Concurrency Model: Pthread-Per-Client up to connection termination limit.
    // Extremely effective model for <50 internal nodes without select() blocking overhead dependencies.
    while((client_sock = accept(server_sock, (struct sockaddr *)&client_args, (socklen_t*)&c))) {
        
        pthread_t thread_id;
        int *new_sock = malloc(1); // Standardized structural casting
        *new_sock = client_sock;
        
        if(pthread_create(&thread_id, NULL, handle_client, (void*) new_sock) < 0) {
            perror("Could not weave isolated execution path block.");
            return 1;
        }
        // Detach safely reclaims internal thread bounds post-exit. 
        pthread_detach(thread_id);
    }

    if (client_sock < 0) {
        perror("Acceptance runtime drastically failed.");
        return 1;
    }

    return 0;
}
