#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_NODES 7
#define INF INT_MAX

// Node mapping: A=0, B=1, C=2, D=3, E=4, F=5, G=6

// Graph Structure (Adjacency Matrix)
typedef struct {
    int adjMatrix[MAX_NODES][MAX_NODES];
    int numVertices;
} Graph;

// Queue for BFS
typedef struct {
    int items[MAX_NODES];
    int front;
    int rear;
} Queue;

// Function to create a graph
void initGraph(Graph* g) {
    g->numVertices = MAX_NODES;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjMatrix[i][j] = 0;
        }
    }
}

// Add edge (undirected) with weight
void addEdge(Graph* g, int src, int dest, int weight) {
    g->adjMatrix[src][dest] = weight;
    g->adjMatrix[dest][src] = weight;
}

// Helper to get node name from index
char getNodeName(int index) {
    return 'A' + index;
}

// Helper to get index from node name
int getNodeIndex(char name) {
    if (name >= 'a' && name <= 'g') return name - 'a';
    if (name >= 'A' && name <= 'G') return name - 'A';
    return -1;
}

// Queue functions
void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(Queue* q) {
    return q->rear == -1;
}

void enqueue(Queue* q, int value) {
    if (q->rear == MAX_NODES - 1) return;
    if (q->front == -1) q->front = 0;
    q->items[++q->rear] = value;
}

int dequeue(Queue* q) {
    if (isEmpty(q)) return -1;
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}

// BFS to find directly connected workstations and analyze risk
void analyze_risk_bfs(Graph* g, int startNode) {
    bool visited[MAX_NODES] = {false};
    Queue q;
    initQueue(&q);

    printf("\n--- Analyzing Risk for Workstation %c ---\n", getNodeName(startNode));

    // Standard BFS initialization
    visited[startNode] = true;
    enqueue(&q, startNode);

    int maxTransferTime = -1;
    int highestRiskNode = -1;

    // In this specific problem, we are primarily interested in the *direct* neighbors (Level 1).
    // The BFS queue will naturally process level 1 immediately after the start node.
    
    // Dequeue the start node
    int current = dequeue(&q); 
    
    printf("Directly Connected Workstations (One-Hop Neighbors):\n");
    
    // Check all adjacent vertices
    int neighborCount = 0;
    for (int i = 0; i < g->numVertices; i++) {
        // If there is an edge
        if (g->adjMatrix[current][i] != 0) {
            int weight = g->adjMatrix[current][i];
            
            printf(" -> Neighbor: %c | Data Transfer: %d minutes\n", getNodeName(i), weight);
            
            // Analyze for highest risk
            if (weight > maxTransferTime) {
                maxTransferTime = weight;
                highestRiskNode = i;
            }

            // Normal BFS continuation (add to queue if not visited)
            // Note: For the specific question "Discover all directly connected", 
            // we technically just iterated them above. 
            // If we needed to traverse the WHOLE graph, we would enqueue here.
            if (!visited[i]) {
                visited[i] = true;
                enqueue(&q, i);
            }
            neighborCount++;
        }
    }

    if (neighborCount == 0) {
        printf("No direct connections found.\n");
    } else {
        printf("\n>>> Risk Analysis Result <<<\n");
        printf("Highest Risk Connected Workstation: %c\n", getNodeName(highestRiskNode));
        printf("Reason: Max data transfer time of %d minutes.\n", maxTransferTime);
    }
}

int main() {
    Graph g;
    initGraph(&g);

    // Initialize Graph based on the diagram
    // Weights approximated from visual if needed, or matched to user's Q3 data where possible.
    // A=0, B=1, C=2, D=3, E=4, F=5, G=6
    
    addEdge(&g, 0, 1, 6); // A-B
    addEdge(&g, 0, 3, 8);   // A-D
    
    addEdge(&g, 1, 2, 11);  // B-C
    addEdge(&g, 1, 3, 5);   // B-D
    
    addEdge(&g, 2, 3, 17);  // C-D
    addEdge(&g, 2, 6, 25);  // C-G
    
    addEdge(&g, 3, 4, 22);  // D-E
    addEdge(&g, 3, 5, 12);  // D-F
    
    addEdge(&g, 4, 5, 10);  // E-F
    
    addEdge(&g, 5, 6, 22);  // F-G

    // Input
    char inputName;
    printf("Enter the suspected compromised workstation (A-G): ");
    scanf(" %c", &inputName);

    int startNode = getNodeIndex(inputName);

    if (startNode == -1) {
        printf("Invalid workstation name. Please enter A-G.\n");
        return 1;
    }

    analyze_risk_bfs(&g, startNode);

    return 0;
}
