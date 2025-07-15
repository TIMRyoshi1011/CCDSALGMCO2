#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 20
#define MAX_LABEL_LEN 9 // 8 chars + null terminator

// Node in adjacency list
typedef struct AdjListNode {
    int vertexIdx; // index in the vertex array
    struct AdjListNode *next;
} AdjListNode;

// Vertex structure
typedef struct Vertex {
    char label[MAX_LABEL_LEN];
    AdjListNode *adjList; // head of adjacency list
    int degree;
} Vertex;

// Graph structure
typedef struct Graph {
    int numVertices;
    Vertex vertices[MAX_VERTICES];
    // For mapping label to index
} Graph;

// Function prototypes
int ReadInputFile(const char *filename, Graph *g);
void ProduceOutputFile1(const char *filename, const Graph *g); // SET
void ProduceOutputFile2(const char *filename, const Graph *g); // DEGREE
void ProduceOutputFile3(const char *filename, const Graph *g); // LIST
void ProduceOutputFile4(const char *filename, const Graph *g); // MATRIX

// Helper functions
int FindVertexIndex(const Graph *g, const char *label);
void FreeGraph(Graph *g);

#endif // GRAPH_H
