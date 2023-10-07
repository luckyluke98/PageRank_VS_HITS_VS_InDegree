#ifndef P_H_
#define P_H_

#include <argp.h>
#include <stdlib.h>

static char args_doc[] = "DATASET FILE NAME";

/* The options we understand. */
static struct argp_option options[] = {
  {"filename-output",   'o', "FILE",    0,  "Output dataset file name" },
  {"benchmark",   'b', 0,    0,  "Compute jaccard coef. between ranking" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
    char * file;
    int order;
    char * output;
    int bench;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state){
    /* Get the input argument from argp_parse, which we
        know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key){
        case 'o':
            arguments->output = arg;
            break;
        case 'b':
            arguments->bench = 1;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1)
                /* Too many arguments. */
                argp_usage (state);

            arguments->file = arg;

            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1)
                /* Not enough arguments. */
                argp_usage (state);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
        }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc};

#endif //P_H_

