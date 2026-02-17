#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 10
#define INF 99999
#define NAME_LEN 5

// Node Mapping
// 0:S1, 1:S2, 2:S3, 3:S4, 4:S5, 5:S6, 6:X

typedef struct {
    char name[NAME_LEN];
    int id;
} Node;

typedef struct {
    int matrix[MAX_NODES][MAX_NODES];
    Node nodes[MAX_NODES];
    int num_nodes;
} Graph;

Graph g;

// Function Prototypes
void init_graph();
int get_node_index(char *name);
void add_edge(char *src, char *dest, int latency);
void dijkstra(int startNode, int endNode);
void print_path(int parent[], int j);

// Initialize Graph
void init_graph() {
    g.num_nodes = 0;
    // Initialize Matrix with INF
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g.matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
    
    // Hardcode Nodes for this specific problem to ensure consistent mapping
    strcpy(g.nodes[0].name, "S1"); g.nodes[0].id = 0;
    strcpy(g.nodes[1].name, "S2"); g.nodes[1].id = 1;
    strcpy(g.nodes[2].name, "S3"); g.nodes[2].id = 2;
    strcpy(g.nodes[3].name, "S4"); g.nodes[3].id = 3;
    strcpy(g.nodes[4].name, "S5"); g.nodes[4].id = 4;
    strcpy(g.nodes[5].name, "S6"); g.nodes[5].id = 5;
    strcpy(g.nodes[6].name, "X");  g.nodes[6].id = 6;
    g.num_nodes = 7;
}

int get_node_index(char *name) {
    for (int i = 0; i < g.num_nodes; i++) {
        if (strcmp(g.nodes[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void add_edge(char *src, char *dest, int latency) {
    int u = get_node_index(src);
    int v = get_node_index(dest);
    
    if (u != -1 && v != -1) {
        g.matrix[u][v] = latency;
        g.matrix[v][u] = latency; // Bidirectional
    } else {
        printf("Error: Invalid nodes %s-%s\n", src, dest);
    }
}

// Recursive Path Printing
void print_path(int parent[], int j) {
    if (parent[j] == -1) return;
    print_path(parent, parent[j]);
    printf(" -> %s", g.nodes[j].name);
}

// Dijkstra's Algorithm
void dijkstra(int startNode, int endNode) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    int parent[MAX_NODES];
    
    // Init
    for (int i = 0; i < g.num_nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    
    dist[startNode] = 0;
    
    // Main Loop
    for (int count = 0; count < g.num_nodes - 1; count++) {
        // Pick min distance vertex not yet visited
        int u = -1, min = INF;
        for (int v = 0; v < g.num_nodes; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }
        
        if (u == -1) break; // unreachable
        
        visited[u] = 1;
        
        // Relax neighbors
        for (int v = 0; v < g.num_nodes; v++) {
            if (!visited[v] && g.matrix[u][v] != INF && dist[u] != INF && 
                dist[u] + g.matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + g.matrix[u][v];
                parent[v] = u;
            }
        }
    }
    
    // Output Result
    if (dist[endNode] == INF) {
        printf("No path found between %s and %s\n", g.nodes[startNode].name, g.nodes[endNode].name);
    } else {
        printf("\n=== Optimal Routing Path ===\n");
        printf("Start: %s\n", g.nodes[startNode].name);
        printf("Path: %s", g.nodes[startNode].name);
        print_path(parent, endNode);
        printf("\n");
        printf("Total Latency: %d ms\n", dist[endNode]);
        printf("============================\n");
    }
}

int main() {
    init_graph();
    
    // Topology from Requirements
    add_edge("S1", "S2", 8);
    add_edge("S1", "S4", 20);
    add_edge("S2", "S3", 7);
    add_edge("S3", "S6", 12);
    add_edge("S4", "S5", 4);
    add_edge("S5", "S6", 6);
    add_edge("S2", "X", 3);
    add_edge("X", "S5", 5);
    
    printf("Network Routing Simulator Initialized.\n");
    printf("Servers: S1, S2, S3, S4, S5, S6, X (Switch)\n");
    
    char src[NAME_LEN], dest[NAME_LEN];
    
    while(1) {
        printf("\nEnter Source Server (or 'exit'): ");
        scanf("%s", src);
        if (strcmp(src, "exit") == 0) break;
        
        printf("Enter Target Server: ");
        scanf("%s", dest);
        
        int u = get_node_index(src);
        int v = get_node_index(dest);
        
        if (u == -1 || v == -1) {
            printf("Error: Invalid server name.\n");
        } else {
            dijkstra(u, v);
        }
    }
    
    return 0;
}
