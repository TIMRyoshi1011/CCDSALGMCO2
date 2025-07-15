#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traversal.h"

// Helper: compare neighbor labels for qsort
static const Graph *g_qsort_ctx = NULL;

static int cmp_neighbor_label(const void *a, const void *b) {
    int ia = *(const int *)a, ib = *(const int *)b;
    return strcmp(g_qsort_ctx->vertices[ia].label, g_qsort_ctx->vertices[ib].label);
}

// BFS traversal with lexicographical order
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

// DFS traversal with lexicographical order
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
