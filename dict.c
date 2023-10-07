#include "dict.h"

#include <stdio.h>
#include <stdlib.h>

struct dictionary new_dict(int dim){
    struct dictionary dict;

    dict.dictionary = (struct dictionary_entry *) calloc(dim, sizeof(struct dictionary_entry));

    if (dict.dictionary == NULL) {
        printf("Error malloc\n");
        exit(1);
    }

    dict.dim = dim;

    for (int i = 0; i < dim; i++) {
        dict.dictionary[i].value = (int *) calloc(1, sizeof(int));

        if (dict.dictionary[i].value == NULL) {
            printf("Error malloc\n");
            exit(1);
        }

        dict.dictionary[i].size = 0;
        dict.dictionary[i].dim = 1;
    }

    return dict;
}

void expand_dict(struct dictionary * dict) {
    dict->dictionary = (struct dictionary_entry *) realloc(dict->dictionary, sizeof(struct dictionary_entry) * dict->dim * 2);

    if (dict->dictionary == NULL) {
        printf("Error Realloc\n");
        exit(1);
    }

    for (int i = dict->dim; i < dict->dim * 2; i++) {

        dict->dictionary[i].value = (int *) calloc(1, sizeof(int));

        if (dict->dictionary[i].value == NULL) {
            printf("Error malloc\n");
            exit(1);
        }

        dict->dictionary[i].size = 0;
        dict->dictionary[i].dim = 1;
    }
    dict->dim = dict->dim * 2;
}

void add_to_dict(struct dictionary * dict, int key, int value) {

    if (key >= dict->dim)
        expand_dict(dict);

    if (dict->dictionary[key].dim == dict->dictionary[key].size) {
        dict->dictionary[key].value = (int *) realloc(dict->dictionary[key].value, sizeof(int) * dict->dictionary[key].dim * 2);
        
        if (dict->dictionary[key].value == NULL) {
            printf("Error Realloc\n");
            exit(1);
        }

        dict->dictionary[key].dim = dict->dictionary[key].dim * 2;
    }

    dict->dictionary[key].key = key;
    dict->dictionary[key].value[dict->dictionary[key].size] = value;
    dict->dictionary[key].size = dict->dictionary[key].size + 1;
}

//#################################################################################################################################################

struct dictionary_f new_dict_f(int dim){
    struct dictionary_f dict;

    dict.dictionary = (struct dictionary_entry_f *) calloc(dim, sizeof(struct dictionary_entry_f));

    if (dict.dictionary == NULL) {
        printf("Error malloc\n");
        exit(1);
    }

    dict.dim = dim;

    for (int i = 0; i < dim; i++) {
        dict.dictionary[i].value = (float *) calloc(1, sizeof(float));

        if (dict.dictionary[i].value == NULL) {
            printf("Error malloc\n");
            exit(1);
        }

        dict.dictionary[i].size = 0;
        dict.dictionary[i].dim = 1;
    }

    return dict;
}

void expand_dict_f(struct dictionary_f * dict) {
    dict->dictionary = (struct dictionary_entry_f *) realloc(dict->dictionary, sizeof(struct dictionary_entry_f) * dict->dim * 2);

    if (dict->dictionary == NULL) {
        printf("Error Realloc\n");
        exit(1);
    }

    for (int i = dict->dim; i < dict->dim * 2; i++) {

        dict->dictionary[i].value = (float *) calloc(1, sizeof(float));

        if (dict->dictionary[i].value == NULL) {
            printf("Error malloc\n");
            exit(1);
        }

        dict->dictionary[i].size = 0;
        dict->dictionary[i].dim = 1;
    }
    dict->dim = dict->dim * 2;
}

void add_to_dict_f(struct dictionary_f * dict, int key, float value) {

    if (key >= dict->dim)
        expand_dict_f(dict);

    if (dict->dictionary[key].dim == dict->dictionary[key].size) {
        dict->dictionary[key].value = (float *) realloc(dict->dictionary[key].value, sizeof(float) * dict->dictionary[key].dim * 2);
        
        if (dict->dictionary[key].value == NULL) {
            printf("Error Realloc\n");
            exit(1);
        }

        dict->dictionary[key].dim = dict->dictionary[key].dim * 2;
    }

    dict->dictionary[key].key = key;
    dict->dictionary[key].value[dict->dictionary[key].size] = value;
    dict->dictionary[key].size = dict->dictionary[key].size + 1;
}

struct dictionary_f create_dict_f(float * vect, int dim){
    struct dictionary_f dict = new_dict_f(dim);

    for (int i = 0; i < dim; i++) {
        //printf("%d %f\n", i, vect[i]);
        add_to_dict_f(&dict, i, vect[i]);
    }

    return dict;
}

int cmp (const void * a, const void * b) {
    struct dictionary_entry_f A = *(struct dictionary_entry_f *) a;
    struct dictionary_entry_f B = *(struct dictionary_entry_f *) b;

    if (A.value[0] > B.value[0]) return -1;
    if (A.value[0] < B.value[0]) return 1;
    return 0;
}

void sort_dict_f(struct dictionary_f * dict) {
    qsort(dict->dictionary, dict->dim, sizeof(struct dictionary_entry_f), cmp);
}