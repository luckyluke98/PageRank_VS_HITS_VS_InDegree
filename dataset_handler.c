#include <stdlib.h>
#include "dataset_handler.h"
#include "dict.h"

void get_nodes_edges(char * filename, int * nodes, int * edges) {
    FILE * fp;
    char c, str[100];

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Cannot open the file");
        exit(1);
    }

    c = getc(fp);

    while (c == '#') {
        fgets(str, 100 - 1, fp);
        sscanf(str, "%*s %d %*s %d", nodes, edges);
        c = getc(fp);
    }
    
    fclose(fp);
}

FILE * skip_comment(char * filename, int * nodes, int * edges) {
    FILE * fp;
    char c, str[100];

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Cannot open the file");
        exit(1);
    }

    c = getc(fp);

    while (c == '#') {
        fgets(str, 100 - 1, fp);
        sscanf(str, "%*s %d %*s %d", nodes, edges);
        c = getc(fp);
    }
    ungetc(c, fp);

    return fp;
}

void order_dataset(char * filename, char * out_filename) {
    FILE * fp, * fp_out;
    
    int fromnode = 0, tonode = 0;
    int old_fromnode = -1, old_tonode = -1;
    int nodes, edges;

    fp = skip_comment(filename, &nodes, &edges);

    int * occ = (int *) calloc(nodes, sizeof(int));

    // Like "Single pass in memory"
    struct dictionary dict = new_dict(nodes);

    fscanf(fp, "%d %d", &fromnode, &tonode);

    while (!feof(fp)) {
        occ[fromnode] = 1;
        occ[tonode] = 1;
        add_to_dict(&dict, tonode, fromnode);
        fscanf(fp, "%d %d", &fromnode, &tonode);
    }

    fclose(fp);

    if ((fp_out = fopen(out_filename, "w")) == NULL) {
        printf("Unable to create file.\n");
        exit(1);
    }

    // Write file 
    int start_with_one = 0;

    if (occ[0] == 0) start_with_one = 1;

    fprintf(fp_out, "# Nodes: %d Edges: %d\n", nodes, edges);

    for (int i = 0; i < dict.dim; i++) {
        for (int j = 0; j < dict.dictionary[i].size; j++) {
            if (start_with_one)
                fprintf(fp_out, "%d\t%d\n", dict.dictionary[i].key - 1, dict.dictionary[i].value[j] - 1);
            else
                fprintf(fp_out, "%d\t%d\n", dict.dictionary[i].key, dict.dictionary[i].value[j]);
        }
    }
    fclose(fp_out);

    // Free Memory
    free_dict(&dict);
    free(occ);
}

void create_CSR(char * filename, float * val, int * col_ind, int * row_ptr) {
    FILE *fp;
    int fromnode, tonode;
    int nodes, edges;

    fp = skip_comment(filename, &nodes, &edges);

    int j, i = 0;
    int elrow = 0, curel = 0, row = 0;

    row_ptr[0] = 0;

    while (!feof(fp) && i < edges) {
        fscanf(fp, "%d%d", &fromnode, &tonode);

        if (fromnode > row) {
            curel = curel + elrow;
            for (int k = row + 1; k <= fromnode; k++) {
                row_ptr[k] = curel;
            }
            elrow = 0;
            row = fromnode;
        }
        val[i] = 1.0;
        col_ind[i] = tonode;
        elrow++;
        i++;
    }
    fclose(fp);
    
    row_ptr[row + 1] = curel + elrow - 1;
    row++;

    while (row < nodes) {
        row_ptr[row + 1] = row_ptr[row];
        row++;
    }
}