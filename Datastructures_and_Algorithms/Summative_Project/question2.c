#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CMD_LEN 50
#define TYPO_THRESHOLD 3 // Max distance to suggest a correction

// BST Node
typedef struct Node {
    char command[MAX_CMD_LEN];
    struct Node *left;
    struct Node *right;
} Node;

// Global variables
Node *root = NULL;

// Function Prototypes
Node* insert(Node *root, char *cmd);
Node* search(Node *root, char *cmd);
void load_commands(const char *filename);
void log_rejection(const char *cmd);
int levenshtein(const char *s1, const char *s2);
void find_closest_command(Node *root, const char *target, char *best_match, int *min_dist);
void free_tree(Node *root);
int min3(int a, int b, int c);

// --- BST Implementations ---

Node* create_node(char *cmd) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory Error\n");
        exit(1);
    }
    strcpy(newNode->command, cmd);
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node *root, char *cmd) {
    if (root == NULL) return create_node(cmd);
    
    int cmp = strcmp(cmd, root->command);
    if (cmp < 0)
        root->left = insert(root->left, cmd);
    else if (cmp > 0)
        root->right = insert(root->right, cmd);
    
    return root;
}

Node* search(Node *root, char *cmd) {
    if (root == NULL || strcmp(root->command, cmd) == 0)
        return root;
    
    if (strcmp(cmd, root->command) < 0)
        return search(root->left, cmd);
    
    return search(root->right, cmd);
}

void free_tree(Node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// --- Levenshtein Distance & Suggestion Logic ---

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

// Standard DP implementation of Levenshtein Distance
int levenshtein(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int matrix[MAX_CMD_LEN][MAX_CMD_LEN];
    
    for (int i = 0; i <= len1; i++) matrix[i][0] = i;
    for (int j = 0; j <= len2; j++) matrix[0][j] = j;
    
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            matrix[i][j] = min3(
                matrix[i-1][j] + 1,      // deletion
                matrix[i][j-1] + 1,      // insertion
                matrix[i-1][j-1] + cost  // substitution
            );
        }
    }
    return matrix[len1][len2];
}

void find_closest_command(Node *root, const char *target, char *best_match, int *min_dist) {
    if (root == NULL) return;
    
    // Check current node
    int dist = levenshtein(target, root->command);
    if (dist < *min_dist) {
        *min_dist = dist;
        strcpy(best_match, root->command);
    }
    
    find_closest_command(root->left, target, best_match, min_dist);
    find_closest_command(root->right, target, best_match, min_dist);
}

// --- File I/O ---

void load_commands(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open command file '%s'. Using defaults.\n", filename);
        // Fallback defaults for simulation if file missing
        root = insert(root, "START_UP");
        root = insert(root, "STOP_CONVEYOR");
        root = insert(root, "RESET_ALARM");
        root = insert(root, "EMERGENCY_STOP");
        root = insert(root, "CHECK_STATUS");
        return;
    }
    
    char buffer[MAX_CMD_LEN];
    while (fgets(buffer, sizeof(buffer), file)) {
        // Trim newline
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) > 0) {
            root = insert(root, buffer);
        }
    }
    fclose(file);
    printf("Commands loaded from %s.\n", filename);
}

void log_rejection(const char *cmd) {
    FILE *file = fopen("review.log", "a");
    if (file) {
        fprintf(file, "Rejected Command: %s\n", cmd);
        fclose(file);
    } else {
        printf("Error: Could not write to log file.\n");
    }
}

// --- Main ---

int main() {
    // 1. Load Commands
    load_commands("commands.txt");
    
    printf("\n=== Industrial Control Terminal ===\n");
    printf("Type a command to execute (or 'exit' to quit).\n");
    
    char input[MAX_CMD_LEN];
    
    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        // Trim newline
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "exit") == 0) break;
        if (strlen(input) == 0) continue;
        
        // 2. Exact Match Check
        Node *result = search(root, input);
        if (result) {
            printf("[SUCCESS] Command '%s' executed.\n", result->command);
        } else {
            // 3. Typo Check
            char best_match[MAX_CMD_LEN] = "";
            int min_dist = 1000;
            
            find_closest_command(root, input, best_match, &min_dist);
            
            if (min_dist <= TYPO_THRESHOLD) {
                printf("[Did you mean?] %s (Typo detected)\n", best_match);
            } else {
                printf("[ERROR] Unrecognized command '%s'. Logged for review.\n", input);
                log_rejection(input);
            }
        }
    }
    
    free_tree(root);
    printf("System Shutdown.\n");
    return 0;
}
