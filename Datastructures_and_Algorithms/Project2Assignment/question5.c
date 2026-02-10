#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 10 // A-J
#define MAX_EDGES 40 // Assuming roughly dense connections
#define INF 999999   // Using a safe infinity that won't overflow when added

// Node Mapping:
// A:0, B:1, C:2, D:3, E:4,
// F:5, G:6, H:7, I:8, J:9

typedef struct {
    int src;
    int dest;
    int weight;
} Edge;

typedef struct {
    int numVertices;
    int numEdges;
    Edge edges[MAX_EDGES];
} Graph;

// Helper to get node name
char getNodeName(int index) {
    return 'A' + index;
}

// Function to initialize graph
void initGraph(Graph* g, int vertices) {
    g->numVertices = vertices;
    g->numEdges = 0;
}

// Add *directed* edge
void addDirectedEdge(Graph* g, int src, int dest, int weight) {
    if (g->numEdges < MAX_EDGES) {
        g->edges[g->numEdges].src = src;
        g->edges[g->numEdges].dest = dest;
        g->edges[g->numEdges].weight = weight;
        g->numEdges++;
    }
}

// Add *undirected* edge (adds two directed edges)
void addEdge(Graph* g, int src, int dest, int weight) {
    addDirectedEdge(g, src, dest, weight);
    addDirectedEdge(g, dest, src, weight);
}

// Bellman-Ford Algorithm
void bellman_ford(Graph* g, int startNode) {
    int dist[MAX_NODES];
    int parent[MAX_NODES];

    // Step 1: Initialize distances from src to all other vertices as INFINITE
    for (int i = 0; i < g->numVertices; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[startNode] = 0;

    // Step 2: Relax all edges |V| - 1 times
    for (int i = 1; i <= g->numVertices - 1; i++) {
        for (int j = 0; j < g->numEdges; j++) {
            int u = g->edges[j].src;
            int v = g->edges[j].dest;
            int weight = g->edges[j].weight;

            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    // Step 3: Check for negative-weight cycles
    for (int j = 0; j < g->numEdges; j++) {
        int u = g->edges[j].src;
        int v = g->edges[j].dest;
        int weight = g->edges[j].weight;

        if (dist[u] != INF && dist[u] + weight < dist[v]) {
            printf("\nGraph contains negative weight cycle! Transaction system is unstable.\n");
            return; // Exit if negative cycle detected
        }
    }

    // Print Solution
    printf("\n--- Minimum Transaction Costs from Branch %c (Central) ---\n", getNodeName(startNode));
    printf("Target Branch | Min Cost | Path\n");
    printf("--------------|----------|-----------------\n");

    for (int i = 0; i < g->numVertices; i++) {
        if (i == startNode) continue;

        printf("      %c       |    %2d    | ", getNodeName(i), dist[i] == INF ? -1 : dist[i]);
        
        if (dist[i] == INF) {
            printf("Unreachable\n");
        } else {
            // Reconstruct path
            int path[MAX_NODES];
            int count = 0;
            int curr = i;
            while (curr != -1) {
                path[count++] = curr;
                curr = parent[curr];
            }
            // Print path in reverse
            for (int k = count - 1; k >= 0; k--) {
                printf("%c", getNodeName(path[k]));
                if (k > 0) printf(" -> ");
            }
            printf("\n");
        }
    }
}

int main() {
    Graph g;
    // Nodes A-J = 10 nodes
    initGraph(&g, 10);

    // Edges based on the diagram provided
    // Weights are approximated where unclear
    
    // Node indices:
    // A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9

    addEdge(&g, 0, 1, 4);   // A-B (4)
    addEdge(&g, 0, 3, 4);   // A-D (Approximated as 4, clear label suggests 4? or could be 16?) -- Using 4 as it looks closer.
                            // Re-evaluating: user prompt says '16' next to D? No wait, label is near line A-D.
                            // Let's check prompts text/image closer. Actually looks like 4.
                            // Wait, looking at B-D = 6, J-B = 7. 
                            // Let's assume A-D is 4 based on visual similarity to A-B.
                            // Actually, let's use user verifiable placeholders if needed.
    
    addEdge(&g, 1, 2, 6);   // B-C (6)
    addEdge(&g, 1, 3, 6);   // B-D (6)
    addEdge(&g, 1, 9, 7);   // B-J (7)
    
    addEdge(&g, 2, 6, 9);   // C-G (9)
    
    addEdge(&g, 3, 4, 7);   // D-E (7)
    addEdge(&g, 3, 9, 1);   // D-J (Weight unclear, using 1 as placeholder "small connection")
    
    
    addEdge(&g, 9, 5, 3);   // J-F (3)
    
    addEdge(&g, 4, 5, 10);  // E-F (10)
    addEdge(&g, 4, 8, 2);   // E-I (2)
    
    addEdge(&g, 5, 6, 1);   // F-G (Weight unclear, using 1)
    addEdge(&g, 5, 8, 10);  // F-I (10)
    
    addEdge(&g, 6, 7, 13);  // G-H (13)

    printf("Banking System Risk Assessment Model Initialized.\n");
    printf("Calculating minimum risk paths...\n");

    // Run Bellman-Ford from Source Node A (0)
    bellman_ford(&g, 0);

    return 0;
}
