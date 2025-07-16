// Author: 
// Tester: Marcus Timothy V. Ramos

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

/* Helper: This function finds the index of vertex by label
 * @param *g - Pointer to the graph structure.
 * @param *label - The label of the vertex to find.
 * @return index (i) if found, -1 otherwise.
 */
int FindVertexIndex(const Graph *g, const char *label) {
    for (int i = 0; i < g->numVertices; ++i) {
        if (strcmp(g->vertices[i].label, label) == 0) {
            return i;
        }
    }
    return -1;
}

/* Helper: Free all adjacency lists
 * @param *g - Pointer to the graph structure.
 */
void FreeGraph(Graph *g) {
    for (int i = 0; i < g->numVertices; ++i) {
        AdjListNode *curr = g->vertices[i].adjList;
        while (curr) {
            AdjListNode *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
        g->vertices[i].adjList = NULL;
    }
    g->numVertices = 0;
}

/* Read input file and build graph
 * @param *filename - The name of the input file.
 * @param *g - Pointer to the graph structure
 * @return 1 if successful, 0 otherwise.
 */
int ReadInputFile(const char *filename, Graph *g) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    int n;
    if (fscanf(fp, "%d\n", &n) != 1 || n < 1 || n > MAX_VERTICES) {
        fclose(fp);
        return 0;
    }
    g->numVertices = n;

    char line[256];
    for (int i = 0; i < n; ++i) {
        if (!fgets(line, sizeof(line), fp)) {
            fclose(fp);
            return 0;
        }
        // Parse the line
        char *token = strtok(line, " \t\r\n");
        if (!token) {
            fclose(fp);
            return 0;
        }
        strncpy(g->vertices[i].label, token, MAX_LABEL_LEN-1);
        g->vertices[i].label[MAX_LABEL_LEN-1] = '\0';
        g->vertices[i].adjList = NULL;
        g->vertices[i].degree = 0;
    }
    // Second pass: build adjacency lists
    rewind(fp);
    fscanf(fp, "%d\n", &n); // skip first line
    for (int i = 0; i < n; ++i) {
        if (!fgets(line, sizeof(line), fp)) {
            fclose(fp);
            return 0;
        }
        char *tokens[1+MAX_VERTICES];
        int t = 0;
        char *token = strtok(line, " \t\r\n");
        while (token && strcmp(token, "-1") != 0 && t < MAX_VERTICES+1) {
            tokens[t++] = token;
            token = strtok(NULL, " \t\r\n");
        }
        // tokens[0] is the vertex label
        int srcIdx = FindVertexIndex(g, tokens[0]);
        for (int j = 1; j < t; ++j) {
            int destIdx = FindVertexIndex(g, tokens[j]);
            if (destIdx != -1) {
                // Add to adjacency list if not already present
                AdjListNode *node = g->vertices[srcIdx].adjList;
                int found = 0;
                while (node) {
                    if (node->vertexIdx == destIdx) { found = 1; break; }
                    node = node->next;
                }
                if (!found) {
                    AdjListNode *newNode = malloc(sizeof(AdjListNode));
                    newNode->vertexIdx = destIdx;
                    newNode->next = g->vertices[srcIdx].adjList;
                    g->vertices[srcIdx].adjList = newNode;
                    g->vertices[srcIdx].degree++;
                }
            }
        }
    }
    fclose(fp);
    return 1;
}

// Use a static context for qsort
static const Graph *g_qsort_ctx = NULL;

/* Helper for qsort: compare vertex labels for sorting
 * @param *a - Pointer to the first vertex index.
 * @param *b - Pointer to the second vertex index.
 * @return negative if a < b, positive if a > b, zero if equal.
 */
static int cmp_vertex_label(const void *a, const void *b) {
    int ia = *(const int *)a, ib = *(const int *)b;
    return strcmp(g_qsort_ctx->vertices[ia].label, g_qsort_ctx->vertices[ib].label);
}

// Helper for qsort: compare edge tuples (by start, then end label)
typedef struct { int u, v; } EdgeTuple;

/* Helper for qsort: compare edge tuples
 * @param *a - Pointer to the first edge tuple.
 * @param *b - Pointer to the second edge tuple.
 * @return negative if a < b, positive if a > b, zero if equal.
*/
static int cmp_edge_tuple(const void *a, const void *b) {
    const EdgeTuple *ea = (const EdgeTuple *)a, *eb = (const EdgeTuple *)b;
    int c = strcmp(g_qsort_ctx->vertices[ea->u].label, g_qsort_ctx->vertices[eb->u].label);
    if (c != 0) return c;
    return strcmp(g_qsort_ctx->vertices[ea->v].label, g_qsort_ctx->vertices[eb->v].label);
}

/* Output File #1: SET
 * This function creates the text file containing the set of vertices V(G) and set of edges E(G)
 * @param *filename - The name of the output file.
 * @param *g - Pointer to the graph structure.
 */
void ProduceOutputFile1(const char *filename, const Graph *g) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    // Vertices sorted by label
    int idx[MAX_VERTICES];
    for (int i = 0; i < g->numVertices; ++i) idx[i] = i;
    g_qsort_ctx = g;
    qsort(idx, g->numVertices, sizeof(int), cmp_vertex_label);
    fprintf(fp, "V(G)={");
    for (int i = 0; i < g->numVertices; ++i) {
        fprintf(fp, "%s", g->vertices[idx[i]].label);
        if (i < g->numVertices - 1) fprintf(fp, ",");
    }
    fprintf(fp, "}\n");
    // Edges: collect unique edges (u < v by label)
    EdgeTuple edges[MAX_VERTICES * MAX_VERTICES];
    int edgeCount = 0;
    for (int i = 0; i < g->numVertices; ++i) {
        AdjListNode *node = g->vertices[i].adjList;
        while (node) {
            int j = node->vertexIdx;
            if (strcmp(g->vertices[i].label, g->vertices[j].label) < 0) {
                edges[edgeCount].u = i;
                edges[edgeCount].v = j;
                ++edgeCount;
            }
            node = node->next;
        }
    }
    g_qsort_ctx = g;
    qsort(edges, edgeCount, sizeof(EdgeTuple), cmp_edge_tuple);
    fprintf(fp, "E(G)={");
    for (int i = 0; i < edgeCount; ++i) {
        fprintf(fp, "(%s,%s)", g->vertices[edges[i].u].label, g->vertices[edges[i].v].label);
        if (i < edgeCount - 1) fprintf(fp, ",");
    }
    fprintf(fp, "}");
    fclose(fp);
}

/* Output File #2: DEGREE
 * This function creates the text file containing the list of vertex IDs with the corresponding degree for each vertex. 
 * @param *filename - The name of the output file.
 * @param *g - Pointer to the graph structure.
 */
void ProduceOutputFile2(const char *filename, const Graph *g) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    int idx[MAX_VERTICES];
    for (int i = 0; i < g->numVertices; ++i) idx[i] = i;
    g_qsort_ctx = g;
    qsort(idx, g->numVertices, sizeof(int), cmp_vertex_label);
    for (int i = 0; i < g->numVertices; ++i) {
        fprintf(fp, "%s %d\n", g->vertices[idx[i]].label, g->vertices[idx[i]].degree);
    }
    fclose(fp);
}

/* Output File #3: LIST
 * This function produces the adjacency list representation of the graph in a text file.
 * @param *filename - The name of the output file.
 * @param *g - Pointer to the graph structure.
 */
void ProduceOutputFile3(const char *filename, const Graph *g) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    for (int i = 0; i < g->numVertices; ++i) {
        fprintf(fp, "%s", g->vertices[i].label);
        // Collect neighbors in input order
        AdjListNode *nodes[MAX_VERTICES];
        int n = 0;
        AdjListNode *node = g->vertices[i].adjList;
        while (node) {
            nodes[n++] = node;
            node = node->next;
        }
        // To preserve input order, reverse the array
        for (int j = n-1; j >= 0; --j) {
            fprintf(fp, "->%s", g->vertices[nodes[j]->vertexIdx].label);
        }
        fprintf(fp, "->\\\n");
    }
    fclose(fp);
}

/* Output File #4: MATRIX
 * This function produces the adjacency matrix representation of the graph in a text file.
 * @param *filename - The name of the output file.
 * @param *g - Pointer to the graph structure.
 */
void ProduceOutputFile4(const char *filename, const Graph *g) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    // Compute max label length
    int maxlen = 0;
    for (int i = 0; i < g->numVertices; ++i) {
        int len = (int)strlen(g->vertices[i].label);
        if (len > maxlen) maxlen = len;
    }
    int width = maxlen + 2; // 2 spaces for padding
    // Print header
    fprintf(fp, "%*s", width, ""); // top-left empty cell
    for (int i = 0; i < g->numVertices; ++i) {
        fprintf(fp, "%*s", width, g->vertices[i].label);
    }
    fprintf(fp, "\n");
    for (int i = 0; i < g->numVertices; ++i) {
        fprintf(fp, "%*s", width, g->vertices[i].label);
        for (int j = 0; j < g->numVertices; ++j) {
            // Check adjacency
            int adj = 0;
            AdjListNode *node = g->vertices[i].adjList;
            while (node) {
                if (node->vertexIdx == j) { adj = 1; break; }
                node = node->next;
            }
            fprintf(fp, "%*d", width, adj);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
