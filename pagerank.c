#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "pagerank.h"
#include "dataset_handler.h"

float * page_rank(float * val, int * col_ind, int * row_ptr, int nodes, int edges) {
    clock_t start, end;
    double total_time;
    start = clock();

    int fromnode, tonode;
    int * out_link = (int *) calloc(nodes, sizeof(int));

    int j, i = 0;

    // Rendiamo la matrice compressa stocastica, inzializziamo il vettore degli outlinks per ogni nodo a zero
    for (int i = 0; i < edges; i++) {
        out_link[col_ind[i]] = out_link[col_ind[i]] + 1;
    }

    // Andiamo a calcoalre i valori della "matrice di adicenza" compressa, 1/Oj (# Oj out-link di j)
    for (int i = 0; i < edges; i++) {
        val[i] = val[i] / out_link[col_ind[i]];
    }

    // Inizializziamo il vettore dei "punteggi"
    float * p = (float *) malloc(nodes * sizeof(float));
    
    for (i = 0; i < nodes; i++)
        p[i] = 1.0 / nodes;

    int looping = 1;
    int k = 0;
    float d = 0.85; 
    int num_elem_row = 0;
    float dandling_nodes = 0;

    while (looping) {
        float * p_new = (float *) calloc(nodes, sizeof(float));

        num_elem_row = 0;
        dandling_nodes = 0;
        int pt = 0;

        // Moltiplication A*p
        for (i = 0; i < nodes; i++) {
            num_elem_row = row_ptr[i + 1] - row_ptr[i];
            for (j = 0; j < num_elem_row; j++) {
                p_new[i] = p_new[i] + (val[pt] * p[col_ind[pt]]);
                pt++;
            }
        }

        for (int j = 0; j < nodes; j++)
            if (out_link[j] == 0)
                dandling_nodes = dandling_nodes + p[j] / nodes;

        for (int i = 0; i < nodes; i++)
            p_new[i] = p_new[i] + dandling_nodes;
        
        for (i = 0; i < nodes; i++)
            p_new[i] = d * p_new[i] + (1.0 - d) / nodes;
        
        // Termination
        float epsilon = 0.0;
        for (i = 0; i < nodes; i++)
            epsilon = epsilon + fabs(p_new[i] - p[i]);
       
        if (epsilon < 0.000001)
            looping = 0;

        for (i = 0; i < nodes; i++)
            p[i] = p_new[i];

        k = k + 1;

        free(p_new);
    }

    end = clock();
    total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n------PAGERANK------\n\nNumber of iteration:\t%d \n", k);
    printf("Time spent:\t\t%f seconds.\n", total_time);

    return p;
}