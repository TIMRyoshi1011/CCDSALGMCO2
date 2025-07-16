// Author: 
// Tester: Marcus Timothy V. Ramos

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traversal.c"                                  //<-------------- changed to include traversal.c and removed graph.h

/* Helper: to build output filenames
 * @param input - The input filename.
 * @param suffix - The suffix to append before the extension.
 * @param out - The output buffer for the constructed filename.
 */
void build_output_filename(const char *input, const char *suffix, char *out) {
    // Remove extension
    const char *dot = strrchr(input, '.');
    int len = dot ? (dot - input) : strlen(input);
    strncpy(out, input, len);
    out[len] = '\0';
    strcat(out, suffix);
    strcat(out, ".TXT");
}

int main() {
    char inputFile[64];
    printf("Input filename: ");
    if (!fgets(inputFile, sizeof(inputFile), stdin)) return 1;
    // Remove trailing newline
    inputFile[strcspn(inputFile, "\r\n")] = 0;

    Graph g;
    if (!ReadInputFile(inputFile, &g)) {
        printf("File %s not found.\n", inputFile);
        return 0;
    }

    // Output files 1-4
    char out1[80], out2[80], out3[80], out4[80];
    build_output_filename(inputFile, "-SET", out1);
    build_output_filename(inputFile, "-DEGREE", out2);
    build_output_filename(inputFile, "-LIST", out3);
    build_output_filename(inputFile, "-MATRIX", out4);
    ProduceOutputFile1(out1, &g);
    ProduceOutputFile2(out2, &g);
    ProduceOutputFile3(out3, &g);
    ProduceOutputFile4(out4, &g);

    // Ask for start vertex
    char startVertex[32];
    printf("Input start vertex for the traversal: ");
    if (!fgets(startVertex, sizeof(startVertex), stdin)) {
        FreeGraph(&g);
        return 1;
    }
    startVertex[strcspn(startVertex, "\r\n")] = 0;
    int startIdx = FindVertexIndex(&g, startVertex);
    if (startIdx == -1) {
        printf("Vertex %s not found.\n", startVertex);
        FreeGraph(&g);
        return 0;
    }
    // Output files 5-6
    char out5[80], out6[80];
    build_output_filename(inputFile, "-BFS", out5);
    build_output_filename(inputFile, "-DFS", out6);
    ProduceOutputFile5(out5, &g, startIdx);
    ProduceOutputFile6(out6, &g, startIdx);
    FreeGraph(&g);
    return 0;
}
