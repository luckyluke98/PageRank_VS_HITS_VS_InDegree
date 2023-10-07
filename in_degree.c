#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "in_degree.h"
#include "dataset_handler.h"
#include "dict.h"

float * in_degree(char *filename) {
    clock_t start, end;
    double total_time;
    start = clock();

    FILE *fp;
    int fromnode, tonode;
    int nodes, edges;

    fp = skip_comment(filename, &nodes, &edges);

    struct dictionary dict = new_dict(nodes);

    fscanf(fp, "%d %d", &fromnode, &tonode);

    while (!feof(fp)) {
        add_to_dict(&dict, fromnode, tonode);
        fscanf(fp, "%d %d", &fromnode, &tonode);
    }

    fclose(fp);

    float * rank = (float *) calloc(nodes, sizeof(float));

    for (int i = 0; i < nodes; i++) {
        rank[i] = (float)dict.dictionary[i].size / (float)(nodes - 1);
    }

    end = clock();
    total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n------INDEGREE------\n\n");
    printf("Time spent:\t\t%f seconds.\n", total_time);
    
    return rank;
}