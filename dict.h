#ifndef DICT_H_
#define DICT_H_

struct dictionary_entry_f {
    int key;
    float * value;
    int dim;
    int size;
};

struct dictionary_f {
    struct dictionary_entry_f * dictionary;
    int dim;
    int * occ;
};

struct dictionary_entry {
    int key;
    int * value;
    int dim;
    int size;
};

struct dictionary {
    struct dictionary_entry * dictionary;
    int dim;
    int * occ;
};

struct dictionary new_dict(int dim);
void expand_dict(struct dictionary * dict);
void add_to_dict(struct dictionary * dict, int key, int value);


struct dictionary_f new_dict_f(int dim);
void expand_dict_f(struct dictionary_f * dict);
void add_to_dict_f(struct dictionary_f * dict, int key, float value);
struct dictionary_f create_dict_f(float * vect, int dim);
int cmp (const void * a, const void * b);
void sort_dict_f(struct dictionary_f * dict);

void free_dict_f(struct dictionary_f * dict);
void free_dict(struct dictionary * dict);

#endif //DICT_H_