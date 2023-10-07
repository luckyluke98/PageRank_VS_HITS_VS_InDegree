#ifndef HITS_H_
#define HITS_H_

float * hits(float * val, int * col_ind, int * row_ptr,
             float * val_T, int * col_ind_T, int * row_ptr_T, 
             int nodes, int edges);

#endif //HITS_H_
