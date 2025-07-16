// Author: 
// Tester: Marcus Timothy V. Ramos

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.c"                                          //<-------------- changed to include graph.c

// Helper: compare vertex labels for qsort
//static const Graph *g_qsort_ctx = NULL;
// static int cmp_vertex_label(const void *a, const void *b) {
//     int ia = *(const int *)a, ib = *(const int *)b;
//     return strcmp(g_qsort_ctx->vertices[ia].label, g_qsort_ctx->vertices[ib].label);
// }
// Helper: compare edge tuples (by start, then end label)               //<-------------- to remove (all commented here are already defined in graph.c)
// typedef struct { int u, v; } EdgeTuple;
// static int cmp_edge_tuple(const void *a, const void *b) {
//     const EdgeTuple *ea = (const EdgeTuple *)a, *eb = (const EdgeTuple *)b;
//     int c = strcmp(g_qsort_ctx->vertices[ea->u].label, g_qsort_ctx->vertices[eb->u].label);
//     if (c != 0) return c;
//     return strcmp(g_qsort_ctx->vertices[ea->v].label, g_qsort_ctx->vertices[eb->v].label);
// }

/* This function checks if edge (label1, label2) exists in G (undirected)
 * @param *G - Pointer to the graph structure.
 * @param *label1 - The label of the first vertex.
 * @param *label2 - The label of the second vertex.
 * @return 1 if edge exists, 0 otherwise.
*/
int edge_exists(const Graph *G, const char *label1, const char *label2) {
    int idx1 = FindVertexIndex(G, label1);
    int idx2 = FindVertexIndex(G, label2);
    if (idx1 == -1 || idx2 == -1) return 0;
    AdjListNode *node = G->vertices[idx1].adjList;
    while (node) {
        if (node->vertexIdx == idx2) return 1;
        node = node->next;
    }
    // For undirected, check the other direction
    node = G->vertices[idx2].adjList;
    while (node) {
        if (node->vertexIdx == idx1) return 1;
        node = node->next;
    }
    return 0;
}

int main() {
    char fileG[64], fileH[64];
    printf("Input first graph filename: ");
    if (!fgets(fileG, sizeof(fileG), stdin)) return 1;
    fileG[strcspn(fileG, "\r\n")] = 0;

    Graph G, H;
    if (!ReadInputFile(fileG, &G)) {
        printf("File %s not found.\n", fileG);
        return 0;
    }

    printf("Input second graph filename: ");
    if (!fgets(fileH, sizeof(fileH), stdin)) return 1;
    fileH[strcspn(fileH, "\r\n")] = 0;

    if (!ReadInputFile(fileH, &H)) {
        printf("File %s not found.\n", fileH);
        FreeGraph(&G);
        return 0;
    }

    // Prepare output filename
    char outFile[128];
    const char *dotG = strrchr(fileG, '.');
    const char *dotH = strrchr(fileH, '.');
    int lenG = dotG ? (dotG - fileG) : strlen(fileG);
    int lenH = dotH ? (dotH - fileH) : strlen(fileH);
    snprintf(outFile, sizeof(outFile), "%.*s-%.*s-SUBGRAPH.TXT", lenG, fileG, lenH, fileH);

    FILE *fp = fopen(outFile, "w");
    if (!fp) {
        FreeGraph(&G); FreeGraph(&H);
        return 1;
    }

    // 1. Vertices in H, alphabetical order, mark + if in G, - if not
    int idxH[MAX_VERTICES];
    for (int i = 0; i < H.numVertices; ++i) idxH[i] = i;
    g_qsort_ctx = &H;
    qsort(idxH, H.numVertices, sizeof(int), cmp_vertex_label);
    int all_vertices_in_G = 1;
    for (int i = 0; i < H.numVertices; ++i) {
        const char *label = H.vertices[idxH[i]].label;
        int found = (FindVertexIndex(&G, label) != -1);
        fprintf(fp, "%s %c\n", label, found ? '+' : '-');
        if (!found) all_vertices_in_G = 0;
    }

    // 2. Edges in H, alphabetical order, mark + if in G, - if not
    // Collect unique edges in H (u < v by label)
    EdgeTuple edgesH[MAX_VERTICES * MAX_VERTICES];
    int edgeCountH = 0;
    for (int i = 0; i < H.numVertices; ++i) {
        AdjListNode *node = H.vertices[i].adjList;
        while (node) {
            int j = node->vertexIdx;
            if (strcmp(H.vertices[i].label, H.vertices[j].label) < 0) {
                edgesH[edgeCountH].u = i;
                edgesH[edgeCountH].v = j;
                ++edgeCountH;
            }
            node = node->next;
        }
    }
    g_qsort_ctx = &H;
    qsort(edgesH, edgeCountH, sizeof(EdgeTuple), cmp_edge_tuple);
    int all_edges_in_G = 1;
    for (int i = 0; i < edgeCountH; ++i) {
        const char *u = H.vertices[edgesH[i].u].label;
        const char *v = H.vertices[edgesH[i].v].label;
        int found = edge_exists(&G, u, v);
        fprintf(fp, "(%s,%s) %c\n", u, v, found ? '+' : '-');
        if (!found) all_edges_in_G = 0;
    }

    // 3. Last line: subgraph status
    // Get graph names (before dot)
    char nameG[32], nameH[32];
    snprintf(nameG, sizeof(nameG), "%.*s", lenG, fileG);
    snprintf(nameH, sizeof(nameH), "%.*s", lenH, fileH);
    if (all_vertices_in_G && all_edges_in_G) {
        fprintf(fp, "%s is a subgraph of %s.\n", nameH, nameG);
    } else {
        fprintf(fp, "%s is a not subgraph of %s.\n", nameH, nameG);
    }
    fclose(fp);
    FreeGraph(&G);
    FreeGraph(&H);
    return 0;
} 