#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "dict.h"
#include "dataset_handler.h"
#include "pagerank.h"
#include "in_degree.h"
#include "hits.h"
#include "benchmarks.h"

int main(int argc, char **argv) {

    int nodes, edges;
    struct arguments arguments;

    // For CSR
    float * val, * val_T;
    int * col_ind, * row_ptr, * col_ind_T, * row_ptr_T;

    // Default values
    arguments.order = 0;
    arguments.output = "results/-out.txt";
    arguments.bench = 0;
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    printf("\nDataset: %s\nOutput: %s\n", arguments.file, arguments.output);

    get_nodes_edges(arguments.file, &nodes, &edges);

    // Create CSR
    val = (float *) calloc(edges, sizeof(float));
    col_ind = (int *) calloc(edges, sizeof(int));
    row_ptr = (int *) calloc(nodes + 1, sizeof(int));

    // CSR Traspose
    val_T = (float *) calloc(edges, sizeof(float));
    col_ind_T = (int *) calloc(edges, sizeof(int));
    row_ptr_T = (int *) calloc(nodes + 1, sizeof(int));

    if (val == NULL || col_ind == NULL || row_ptr == NULL ||
        val_T == NULL || col_ind_T == NULL || row_ptr_T == NULL) {
        printf("Error calloc\n");
        exit(1);
    }

    order_dataset(arguments.file, arguments.output);

    create_CSR(arguments.output, val_T, col_ind_T, row_ptr_T); 
    create_CSR(arguments.file, val, col_ind, row_ptr);

    // Rank functions
    float * pr_rank = page_rank(val_T, col_ind_T, row_ptr_T, nodes, edges);
    float * h_rank = hits(val, col_ind, row_ptr, val_T, col_ind_T, row_ptr_T, nodes, edges);
    float * id_rank = in_degree(arguments.output);

    printf("--------------------\n");

    if (arguments.bench) {

        // Benchmarks
        struct dictionary_f pr_dict = create_dict_f(pr_rank, nodes);
        struct dictionary_f id_dict = create_dict_f(id_rank, nodes);
        struct dictionary_f h_dict = create_dict_f(h_rank, nodes);

        sort_dict_f(&pr_dict);
        sort_dict_f(&id_dict);
        sort_dict_f(&h_dict);

        benchmark(arguments.output, pr_dict, id_dict, h_dict);

        free_dict_f(&pr_dict);
        free_dict_f(&id_dict);
        free_dict_f(&h_dict);
    }

    // Free memory
    free(val);
    free(col_ind);
    free(row_ptr);
    free(val_T);
    free(col_ind_T);
    free(row_ptr_T);

    free(pr_rank);
    free(h_rank);
    free(id_rank);
    
    return 1;

}