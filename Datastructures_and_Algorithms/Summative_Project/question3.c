#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 30
#define ID_LEN 10

// Graph Structure
typedef struct {
    char user_ids[MAX_USERS][ID_LEN]; // Map Index -> User ID
    int matrix[MAX_USERS][MAX_USERS]; // Adjacency Matrix (Directed)
    int num_users;
} Graph;

// Global Graph
Graph g;

// Function Prototypes
void init_graph();
int get_or_create_user_index(char *id);
int get_user_index(char *id);
void add_interaction(char *src, char *dest);
void display_matrix();
void query_user(char *id);

// Initialize Graph
void init_graph() {
    g.num_users = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        for (int j = 0; j < MAX_USERS; j++) {
            g.matrix[i][j] = 0;
        }
    }
}

// Find index of user, or create if not exists
int get_or_create_user_index(char *id) {
    // 1. Search existing
    for (int i = 0; i < g.num_users; i++) {
        if (strcmp(g.user_ids[i], id) == 0) {
            return i;
        }
    }
    
    // 2. Add new if capacity allows
    if (g.num_users < MAX_USERS) {
        strcpy(g.user_ids[g.num_users], id);
        return g.num_users++;
    }
    
    printf("Error: Max user capacity reached.\n");
    return -1;
}

// Find index of user (start query)
int get_user_index(char *id) {
    for (int i = 0; i < g.num_users; i++) {
        if (strcmp(g.user_ids[i], id) == 0) {
            return i;
        }
    }
    return -1;
}

// Add Directed Edge
void add_interaction(char *src, char *dest) {
    int u = get_or_create_user_index(src);
    int v = get_or_create_user_index(dest);
    
    if (u != -1 && v != -1) {
        if (g.matrix[u][v] == 0) { // Only log if new
             g.matrix[u][v] = 1;
             printf("Interaction Added: %s -> %s\n", src, dest);
        } else {
             printf("Interaction already exists: %s -> %s\n", src, dest);
        }
    }
}

// Display Adjacency Matrix
void display_matrix() {
    printf("\n=== Adjacency Matrix (Directed Interactions) ===\n");
    
    // Header
    printf("      ");
    for (int i = 0; i < g.num_users; i++) {
        printf("%-6s", g.user_ids[i]);
    }
    printf("\n");
    
    // Rows
    for (int i = 0; i < g.num_users; i++) {
        printf("%-6s", g.user_ids[i]); // Row Label
        for (int j = 0; j < g.num_users; j++) {
            printf("%-6d", g.matrix[i][j]);
        }
        printf("\n");
    }
    printf("================================================\n");
}

// Query User Interactions
void query_user(char *id) {
    int idx = get_user_index(id);
    
    if (idx == -1) {
        printf("[ERROR] User ID '%s' not found.\n", id);
        return;
    }
    
    printf("\n--- Profile: %s ---\n", id);
    
    // Outgoing (Who they interact with)
    printf("Interacts WITH (Outgoing):\n");
    int out_count = 0;
    for (int j = 0; j < g.num_users; j++) {
        if (g.matrix[idx][j] == 1) {
            printf("  -> %s\n", g.user_ids[j]);
            out_count++;
        }
    }
    if (out_count == 0) printf("  (None)\n");
    
    // Incoming (Who interacts with them)
    printf("Interacted BY (Incoming):\n");
    int in_count = 0;
    for (int i = 0; i < g.num_users; i++) {
        if (g.matrix[i][idx] == 1) {
            printf("  <- %s\n", g.user_ids[i]);
            in_count++;
        }
    }
    if (in_count == 0) printf("  (None)\n");
    printf("-------------------\n");
}

int main() {
    init_graph();
    
    printf("Initializing System with Default Data...\n");
    // Default Data from Question
    add_interaction("U101", "U102");
    add_interaction("U101", "U103");
    add_interaction("U102", "U104");
    add_interaction("U103", "U105");
    add_interaction("U104", "U105");
    add_interaction("U104", "U106");
    add_interaction("U105", "U107");
    add_interaction("U106", "U108");
    
    int choice;
    char id1[ID_LEN], id2[ID_LEN];
    
    while(1) {
        printf("\n1. Show Adjacency Matrix\n");
        printf("2. Query User\n");
        printf("3. Add New Interaction\n");
        printf("4. Exit\n");
        printf("Select: ");
        if (scanf("%d", &choice) != 1) break;
        
        switch(choice) {
            case 1:
                display_matrix();
                break;
            case 2:
                printf("Enter User ID to Query: ");
                scanf("%s", id1);
                query_user(id1);
                break;
            case 3:
                printf("Enter Source User ID: ");
                scanf("%s", id1);
                printf("Enter Destination User ID: ");
                scanf("%s", id2);
                add_interaction(id1, id2);
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid Choice.\n");
        }
    }
    
    return 0;
}
