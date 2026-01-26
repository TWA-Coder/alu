#include <stdio.h>
#include <stdlib.h>

// Definition of the Tree Node
typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

// Function Prototypes
Node* createNode(int data);
Node* insert(Node* root, int data);
void printLeaves(Node* root);
void searchNode(Node* root, int data, Node** target, Node** parent);
void printSiblings(Node* parent, int targetData);
void printGrandchildren(Node* target);
void freeTree(Node* root);

// Create a new node with dynamic memory allocation
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Insert a node into the BST (Insertion Rule: BST)
// Smaller values go to the left, larger values go to the right.
Node* insert(Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    // Duplicate values are ignored based on "distinct integers" requirement
    return root;
}

// Identify and print all leaf nodes (nodes with no children)
void printLeaves(Node* root) {
    if (root == NULL) return;
    
    // If it's a leaf
    if (root->left == NULL && root->right == NULL) {
        printf("%d ", root->data);
        return;
    }
    
    printLeaves(root->left);
    printLeaves(root->right);
}

// Search for a node and track its parent
void searchNode(Node* root, int data, Node** target, Node** parent) {
    if (root == NULL) {
        *target = NULL;
        return;
    }
    
    if (root->data == data) {
        *target = root;
        return;
    }
    
    *parent = root; // Update parent before moving down
    if (data < root->data) {
        searchNode(root->left, data, target, parent);
    } else {
        searchNode(root->right, data, target, parent);
    }
}

// Print siblings of the target node
void printSiblings(Node* parent, int targetData) {
    if (parent == NULL) {
        printf("Node has no parent, thus no siblings (Root node).\n");
        return;
    }
    
    if (parent->left != NULL && parent->left->data == targetData) {
        // Target is left child, sibling is right child
        if (parent->right != NULL) {
            printf("Sibling: %d\n", parent->right->data);
        } else {
            printf("No sibling (Only child).\n");
        }
    } else if (parent->right != NULL && parent->right->data == targetData) {
        // Target is right child, sibling is left child
        if (parent->left != NULL) {
            printf("Sibling: %d\n", parent->left->data);
        } else {
            printf("No sibling (Only child).\n");
        }
    }
}

// Print grandchildren of the target node
void printGrandchildren(Node* target) {
    int hasGrandchildren = 0;
    
    printf("Grandchildren: ");
    if (target->left != NULL) {
        if (target->left->left != NULL) {
            printf("%d ", target->left->left->data);
            hasGrandchildren = 1;
        }
        if (target->left->right != NULL) {
            printf("%d ", target->left->right->data);
            hasGrandchildren = 1;
        }
    }
    
    if (target->right != NULL) {
        if (target->right->left != NULL) {
            printf("%d ", target->right->left->data);
            hasGrandchildren = 1;
        }
        if (target->right->right != NULL) {
            printf("%d ", target->right->right->data);
            hasGrandchildren = 1;
        }
    }
    
    if (!hasGrandchildren) {
        printf("None");
    }
    printf("\n");
}

// Recursively free memory
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node* root = NULL;
    int choice, searchVal;
    
    // Array of 68 distinct integers
    int data[68] = {
        500, 250, 750, 100, 300, 600, 800, 50, 150, 275, 
        350, 550, 650, 775, 850, 25, 75, 125, 175, 260, 
        290, 325, 375, 525, 575, 625, 675, 760, 790, 825, 
        875, 10, 30, 60, 80, 110, 130, 160, 180, 255, 
        265, 280, 295, 310, 330, 360, 380, 510, 530, 560, 
        580, 610, 630, 660, 680, 755, 765, 780, 795, 810, 
        830, 860, 880, 5, 15, 35, 45, 90
    };

    // Construct the tree
    printf("Constructing Binary Tree from 68 integers...\n");
    for (int i = 0; i < 68; i++) {
        root = insert(root, data[i]);
    }
    printf("Tree construction complete.\n\n");

    do {
        printf("=== Binary Tree Operations ===\n");
        printf("1. Print Root Node\n");
        printf("2. Print Leaves\n");
        printf("3. Query Specific Node (Parent, Siblings, Grandchildren)\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // Clear invalid input
            choice = 0;
        }

        switch (choice) {
            case 1:
                if (root != NULL) {
                    printf("Root Node: %d\n\n", root->data);
                } else {
                    printf("Tree is empty.\n\n");
                }
                break;
            case 2:
                printf("Leaf Nodes: ");
                printLeaves(root);
                printf("\n\n");
                break;
            case 3:
                printf("Enter node value to query: ");
                if (scanf("%d", &searchVal) != 1) {
                    while(getchar() != '\n'); 
                    printf("Invalid input.\n");
                    break;
                }
                
                Node* target = NULL;
                Node* parent = NULL;
                searchNode(root, searchVal, &target, &parent);
                
                if (target != NULL) {
                    printf("--- Node %d Details ---\n", target->data);
                    
                    // Parent
                    if (parent != NULL) {
                        printf("Parent: %d\n", parent->data);
                    } else {
                        printf("Parent: None (Root Node)\n");
                    }
                    
                    // Siblings
                    printSiblings(parent, target->data);
                    
                    // Grandchildren
                    printGrandchildren(target);
                    printf("-----------------------\n\n");
                } else {
                    printf("Node %d not found in the tree.\n\n", searchVal);
                }
                break;
            case 4:
                printf("Exiting and freeing memory...\n");
                freeTree(root);
                break;
            default:
                printf("Invalid choice. Please try again.\n\n");
        }
    } while (choice != 4);

    return 0;
}
