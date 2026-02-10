#include <stdio.h>
#include <stdlib.h>

#define V 7   // Number of Hubs (Nodes): A, B, C, D, E, F, G
#define MAX_EDGES 20

// Edge structure
typedef struct {
    int src, dest, weight;
} Edge;

// Graph structure
typedef struct {
    int num_vertices;
    int num_edges;
    Edge edges[MAX_EDGES];
} Graph;

// Subset structure for Union-Find
typedef struct {
    int parent;
    int rank;
} Subset;

// Function to find set of an element i (path compression)
int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Function to do union of two sets (union by rank)
void union_sets(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Comparator for qsort
int compare_edges(const void* a, const void* b) {
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    return edgeA->weight - edgeB->weight;
}

// Function to convert index to Node Name (0->A, 1->B, ...)
char get_node_name(int index) {
    return 'A' + index;
}

// Function to print Adjacency Matrix
void print_adjacency_matrix(Graph* graph) {
    int matrix[V][V];
    
    // Initialize with 0
    for(int i=0; i<V; i++) {
        for(int j=0; j<V; j++) {
            matrix[i][j] = 0;
        }
    }
    
    // Fill Matrix
    for(int i=0; i < graph->num_edges; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int w = graph->edges[i].weight;
        matrix[u][v] = w;
        matrix[v][u] = w; // Undirected
    }
    
    printf("\n--- Adjacency Matrix (Cost in Millions) ---\n");
    printf("   ");
    for(int i=0; i<V; i++) printf(" %c ", get_node_name(i));
    printf("\n");
    
    for(int i=0; i<V; i++) {
        printf(" %c ", get_node_name(i));
        for(int j=0; j<V; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------------\n");
}

// Kruskal's Algorithm
void kruskal_algo(Graph* graph) {
    int e = 0; // Index for result[]
    int i = 0; // Index for sorted edges
    int total_cost = 0;
    
    Edge result[V]; // To store the MST edges
    
    // Step 1: Sort all edges in non-decreasing order of their weight
    qsort(graph->edges, graph->num_edges, sizeof(Edge), compare_edges);
    
    // Allocate memory for creating V subsets
    Subset* subsets = (Subset*) malloc(V * sizeof(Subset));
    
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
    
    printf("\n--- Kruskal's Algorithm: Selecting Edges for MST ---\n");
    
    while (e < V - 1 && i < graph->num_edges) {
        // Step 2: Pick the smallest edge
        Edge next_edge = graph->edges[i++];
        
        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);
        
        // If including this edge doesn't cause cycle
        if (x != y) {
            result[e++] = next_edge;
            union_sets(subsets, x, y);
            printf("Selected: %c - %c (Cost: %d)\n", 
                   get_node_name(next_edge.src), 
                   get_node_name(next_edge.dest), 
                   next_edge.weight);
            total_cost += next_edge.weight;
        } else {
             printf("Discarded: %c - %c (Cost: %d) -> Creates Cycle\n", 
                   get_node_name(next_edge.src), 
                   get_node_name(next_edge.dest), 
                   next_edge.weight);
        }
    }
    
    // Print the MST
    printf("\n--- Minimum Spanning Tree (Final Connections) ---\n");
    for (i = 0; i < e; ++i) {
        printf("%c -- %c  ==  %d million\n", 
               get_node_name(result[i].src), 
               get_node_name(result[i].dest), 
               result[i].weight);
    }
    printf("-------------------------------------------------\n");
    printf("Total Installation Cost: %d million dollars\n", total_cost);
    printf("-------------------------------------------------\n");
    
    free(subsets);
}

int main() {
    Graph graph;
    graph.num_vertices = V;
    
    // Define Edges (Source, Dest, Weight)
    // Nodes: A=0, B=1, C=2, D=3, E=4, F=5, G=6
    
    // IMPORTANT: Verify these weights against the Question Image
    Edge edges[] = {
        {0, 1, 6},  // A-B
        {0, 3, 10}, // A-D (ESTIMATED - PLEASE VERIFY)
        {1, 2, 11}, // B-C
        {1, 3, 5},  // B-D
        {2, 3, 17}, // C-D
        {2, 6, 25}, // C-G
        {3, 4, 22}, // D-E
        {3, 6, 22}, // D-G (ESTIMATED - might be D-F or similar path?)
        {3, 5, 12}, // D-F (ESTIMATED - PLEASE VERIFY)
        {4, 5, 10}, // E-F
        {5, 6, 15}  // F-G (ESTIMATED - PLEASE VERIFY)
    };
    
    graph.num_edges = sizeof(edges) / sizeof(edges[0]);
    for(int i=0; i<graph.num_edges; i++) {
        graph.edges[i] = edges[i];
    }
    
    printf("Network Optimization System Initialized.\n");
    
    // 1. Display Adjacency Matrix
    print_adjacency_matrix(&graph);
    
    // 2. Compute MST using Kruskal's
    kruskal_algo(&graph);
    
    return 0;
}
