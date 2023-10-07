#include "benchmarks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int compare(const void* num1, const void* num2) {

    int a = *(int*) num1;  
    int b = *(int*) num2;  
    if(a > b) {  
        return -1;  
    }  
    else if(a < b) {  
        return 1;  
    }  
    return 0;  
}  
 

double intersect_len(int * set_a, int * set_b, int dim) {

    int p_a = 0, p_b = 0;
    int size = 0;

    while(p_a < dim && p_b < dim) {

        if (set_a[p_a] == set_b[p_b]) {
            size++;
            p_a++;
            p_b++;
        }
        else if (set_a[p_a] < set_b[p_b])
            p_b++;
        else 
            p_a++;
    }
    return size;
}

double union_len(int * set_a, int * set_b, int dim) {
    int p_a = 0, p_b = 0;
    int size = 0;

    while(p_a < dim && p_b < dim) {

        if (set_a[p_a] == set_b[p_b]) {
            size++;
            p_a++;
            p_b++;
        }
        else if (set_a[p_a] < set_b[p_b]) {
            p_b++;
            size++;
        }
        else {
            p_a++;
            size++;
        }

    }
    return size;
}

double jaccard(int * set_a, int * set_b, int dim) {
    //printf("%f %f\n", intersect_len(set_a, set_b, dim), union_len(set_a, set_b, dim));

    return intersect_len(set_a, set_b, dim) / union_len(set_a, set_b, dim);
}

void benchmark(char * filename, struct dictionary_f pr_dict, struct dictionary_f id_dict, struct dictionary_f h_dict) {

    int k = 1; // top-k
    int nodes = pr_dict.dim;

    FILE * fp;

    char * out_file = (char *) malloc(strlen(filename) - 8 * sizeof(char));
    memcpy(out_file, filename, strlen(filename) - 8);
    strcat(out_file, "-res.txt");

    printf("Output file: %s\nIt may take several minutes ...\n", out_file);

    if ((fp = fopen(out_file, "w")) == NULL) {
        printf("Unable to create file.\n");
        exit(1);
    }

    while (k < nodes) {
        //printf("%d\n", k);

        int * pr_rank = (int *) malloc(k * sizeof(int));
        int * id_rank = (int *) malloc(k * sizeof(int));
        int * h_rank = (int *) malloc(k * sizeof(int));

        // Create top-k sets
        for (int i = 0; i < k; i++) {
            pr_rank[i] = pr_dict.dictionary[i].key;
            id_rank[i] = id_dict.dictionary[i].key;
            h_rank[i] = h_dict.dictionary[i].key;
        }

        qsort(pr_rank, k, sizeof(int), compare);  
        qsort(id_rank, k, sizeof(int), compare);  
        qsort(h_rank, k, sizeof(int), compare);  

        // Jaccard PR and ID
        double j_pr_id = jaccard(pr_rank, id_rank, k);
        // Jaccard PR and HITS
        double j_pr_h = jaccard(pr_rank, h_rank, k);
        // Jaccard HITS and ID
        double j_h_id = jaccard(h_rank, id_rank, k);
        
        fprintf(fp, "%d %lf\n", k, j_pr_id);
        fprintf(fp, "%d %lf\n", k, j_pr_h);
        fprintf(fp, "%d %lf\n", k, j_h_id);

        free(pr_rank);
        free(id_rank);
        free(h_rank);

        k = k + 10;
    }
    fclose(fp);

}
