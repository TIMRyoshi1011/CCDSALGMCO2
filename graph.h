// Author: 
// Tester: Marcus Timothy V. Ramos

#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 20 // Maximum number of vertices in the graph
#define MAX_LABEL_LEN 9 // 8 chars + null terminator - maximum length of vertex ID

// Node in adjacency list
typedef struct AdjListNode {
    int vertexIdx; // index in the vertex array
    struct AdjListNode *next; // pointer to the next node
} AdjListNode;

// Vertex structure
typedef struct Vertex {
    char label[MAX_LABEL_LEN]; // vertex label (ID)
    AdjListNode *adjList; // head of adjacency list
    int degree; // degree of the vertex
} Vertex;

// Graph data structure
typedef struct Graph {
    int numVertices; // number of vertices in the graph
    Vertex vertices[MAX_VERTICES]; // array to store vertices
    // For mapping label to index
} Graph;

// Function prototypes for the graph
int ReadInputFile(const char *filename, Graph *g);
void ProduceOutputFile1(const char *filename, const Graph *g); // SET
void ProduceOutputFile2(const char *filename, const Graph *g); // DEGREE
void ProduceOutputFile3(const char *filename, const Graph *g); // LIST
void ProduceOutputFile4(const char *filename, const Graph *g); // MATRIX

// Helper functions
int FindVertexIndex(const Graph *g, const char *label);
void FreeGraph(Graph *g);

#endif // GRAPH_H
