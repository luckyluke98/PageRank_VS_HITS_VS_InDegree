#ifndef DSH_H_
#define DSH_H_

#include <stdio.h>

void get_nodes_edges(char * filename, int * nodes, int * edges);
FILE * skip_comment(char * filename, int * nodes, int * edges);
void order_dataset(char * filename, char * out_filename);
void create_CSR(char * filename, float * val, int * col_ind, int * row_ptr);


#endif //DSH_H_