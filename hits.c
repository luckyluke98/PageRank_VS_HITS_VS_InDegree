#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "hits.h"

float * hits(float * val, int * col_ind, int * row_ptr,
             float * val_T, int * col_ind_T, int * row_ptr_T, 
             int nodes, int edges) {
    
    clock_t start, end;
    double total_time;
    start = clock();

    float * a = (float *) malloc(nodes * sizeof(float));
    float * h = (float *) malloc(edges * sizeof(float)); 

    if (a == NULL || h == NULL) {
        printf("Error malloc\n");
        exit(1);
    }

    // Inizializiamo a e h
    for (int i = 0; i < nodes; i++) {
        a[i] = 1.0;
        h[i] = 1.0;
    }

    int looping = 1, k = 0;

    // Power iteration method
    while (looping) {

        float * a_new = (float *) calloc(nodes, sizeof(float));
        float * h_new = (float *) calloc(nodes, sizeof(float)); 

        if (a_new == NULL || h_new == NULL) {
            printf("Error calloc\n");
            exit(1);
        }

        int pt = 0, num_elem_row = 0;

        // Calcoliamo h_new, L * a
        for (int i = 0; i < nodes; i++) {
            num_elem_row = row_ptr[i + 1] - row_ptr[i];
            for (int j = 0; j < num_elem_row; j++) {
                h_new[i] = h_new[i] + (val[pt] * a[col_ind[pt]]);
                pt++;
            }
        }

        pt = 0, num_elem_row = 0;

        // Calcoliamo a_new, L^t * h
        for (int i = 0; i < nodes; i++) {
            num_elem_row = row_ptr_T[i + 1] - row_ptr_T[i];
            for (int j = 0; j < num_elem_row; j++) {
                a_new[i] = a_new[i] + (val_T[pt] * h[col_ind_T[pt]]);
                pt++;
            }
        }

        float sum_a_new = 0.0, sum_h_new = 0.0;

        for (int i = 0; i < nodes; i++) {
            sum_a_new = sum_a_new + a_new[i];
            sum_h_new = sum_h_new + h_new[i];
        }

        // Normalize
        for (int i = 0; i < nodes; i++) {
            a_new[i] = a_new[i] / sum_a_new;
            h_new[i] = h_new[i] / sum_h_new;
        }

        // Compute norm 1 between the vectors a and a_new, and h and h_new
        float epsilon_a = 0.0, epsilon_h = 0.0;
        for (int i = 0; i < nodes; i++) {
            epsilon_a = epsilon_a + fabs(a_new[i] - a[i]);
            epsilon_h = epsilon_h + fabs(h_new[i] - h[i]);
        }

        if (epsilon_a < 0.000001 && epsilon_h < 0.000001)
            looping = 0;

        for (int i = 0; i < nodes; i++) {
            a[i] = a_new[i];
            h[i] = h_new[i];
        }

        k = k + 1;

        free(a_new);
        free(h_new);
    }

    end = clock();
    total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n--------HITS--------\n\nNumber of iteration:\t%d \n", k);
    printf("Time spent:\t\t%f seconds.\n", total_time);

    return a;

}