// Author: 
// Tester: Marcus Timothy V. Ramos

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traversal.h"
#include "graph.c"                              //<-------------- changed to include graph.c

// Helper: compare neighbor labels for qsort
//static const Graph *g_qsort_ctx = NULL;       //<-------------- to remove (already defined in graph.c)

/* Helper for qsort: compare neighbor labels for sorting
 * @param *a - Pointer to the first vertex index.
 * @param *b - Pointer to the second vertex index.
 * @return negative if a < b, positive if a > b, zero if equal.
 */
static int cmp_neighbor_label(const void *a, const void *b) {
    int ia = *(const int *)a, ib = *(const int *)b;
    return strcmp(g_qsort_ctx->vertices[ia].label, g_qsort_ctx->vertices[ib].label);
}

/* Output File #5: BFS traversal with lexicographical order
 * This function creates the text file containing the breadth-first search (BFS) traversal sequence
 * @param *filename - The name of the output file.
 * @param *g - Pointer to the graph structure.
 * @param startIdx - Index of the starting vertex for BFS traversal.
 */
void ProduceOutputFile5(const char *filename, const Graph *g, int startIdx) {
    int visited[MAX_VERTICES] = {0};
    int queue[MAX_VERTICES], front = 0, rear = 0;
    int order[MAX_VERTICES], orderLen = 0;
    queue[rear++] = startIdx;
    visited[startIdx] = 1;
    while (front < rear) {
        int u = queue[front++];
        order[orderLen++] = u;
        // Collect neighbors
        int neighbors[MAX_VERTICES], n = 0;
        AdjListNode *node = g->vertices[u].adjList;
        while (node) {
            if (!visited[node->vertexIdx]) {
                neighbors[n++] = node->vertexIdx;
            }
            node = node->next;
        }
        // Sort neighbors lexicographically
        g_qsort_ctx = g;
        qsort(neighbors, n, sizeof(int), cmp_neighbor_label);
        for (int i = 0; i < n; ++i) {
            if (!visited[neighbors[i]]) {
                queue[rear++] = neighbors[i];
                visited[neighbors[i]] = 1;
            }
        }
    }
    // Output
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    for (int i = 0; i < orderLen; ++i) {
        fprintf(fp, "%s", g->vertices[order[i]].label);
        if (i < orderLen - 1) fprintf(fp, " ");
    }
    fclose(fp);
}

/* This function performs a depth-first search (DFS) traversal starting from the given vertex index.
 * @param *g - Pointer to the graph structure.
 * @param u - Current vertex index.
 * @param *visited - Array to track visited vertices.
 * @param *order - Array to store the DFS traversal order.
 * @param *orderLen - Pointer to the current length of the order array.
 */
static void dfs(const Graph *g, int u, int *visited, int *order, int *orderLen) {
    visited[u] = 1;
    order[(*orderLen)++] = u;
    // Collect neighbors
    int neighbors[MAX_VERTICES], n = 0;
    AdjListNode *node = g->vertices[u].adjList;
    while (node) {
        if (!visited[node->vertexIdx]) {
            neighbors[n++] = node->vertexIdx;
        }
        node = node->next;
    }
    // Sort neighbors lexicographically
    g_qsort_ctx = g;
    qsort(neighbors, n, sizeof(int), cmp_neighbor_label);
    for (int i = 0; i < n; ++i) {
        if (!visited[neighbors[i]]) {
            dfs(g, neighbors[i], visited, order, orderLen);
        }
    }
}

/* Output File #6: DFS traversal with lexicographical order
 * This function creates the text file containing the depth-first search (DFS) traversal sequence
 * @param *filename - The name of the output file.
 * @param *g - Pointer to the graph structure.
 * @param startIdx - Index of the starting vertex for DFS traversal.
 */
void ProduceOutputFile6(const char *filename, const Graph *g, int startIdx) {
    int visited[MAX_VERTICES] = {0};
    int order[MAX_VERTICES], orderLen = 0;
    dfs(g, startIdx, visited, order, &orderLen);
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    for (int i = 0; i < orderLen; ++i) {
        fprintf(fp, "%s", g->vertices[order[i]].label);
        if (i < orderLen - 1) fprintf(fp, " ");
    }
    fclose(fp);
}
