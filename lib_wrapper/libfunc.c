#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "arg_struct.h"
#include "../lib_input.h"

int lib_func(struct lib_input *x)
{
    int ret;

    struct lib_enter_args args = {0};
    struct lib_enter_args *args_addr = &args;
    populate_arg(args_addr, x);
    populate_ret(args_addr, ret);

    // struct lib_input *
    args.entity_metadata[0] = 0;
    args.entity_metadata[1] = sizeof(struct lib_input *);
    args.entity_metadata[2] = 2;

    args.entity_metadata[3] = 1;
    args.entity_metadata[4] = sizeof(struct lib_input);
    args.entity_metadata[5] = 2;
    args.entity_metadata[6] = 10; // child 1 (int *) index
    args.entity_metadata[7] = offsetof(struct lib_input, i_ptr); // child 1 (int *) offset;
    args.entity_metadata[8] = 18; // child 2 (double *) index;
    args.entity_metadata[9] = offsetof(struct lib_input, d_ptr); // child 2 (double *) offset;

    args.entity_metadata[10] = 2;
    args.entity_metadata[11] = sizeof(int *);
    args.entity_metadata[12] = 1;
    args.entity_metadata[13] = 15; // child 1 (int)
    args.entity_metadata[14] = 0;

    args.entity_metadata[15] = 3;
    args.entity_metadata[16] = sizeof(int);
    args.entity_metadata[17] = 0;
    
    args.entity_metadata[18] = 4;
    args.entity_metadata[19] = sizeof(double *);
    args.entity_metadata[20] = 1;
    args.entity_metadata[21] = 23; // child 1 (double)
    args.entity_metadata[22] = 0;

    args.entity_metadata[23] = 5;
    args.entity_metadata[24] = sizeof(double);
    args.entity_metadata[25] = 0;

    args.entity_metadata_size = 26;
    args.arg_entity_index[0] = 0;

    // invoke syscall 888
    struct lib_enter_args *new_args = syscall(888, args_addr);

    int num_args = new_args->num_args;
    struct lib_input *new_x = *((struct lib_input **)new_args->args[0]);

    int *new_i_ptr = new_x->i_ptr;
    int new_i = *new_i_ptr;
    double *new_d_ptr = new_x->d_ptr;
    double new_d = *new_d_ptr;
    float new_f = new_x->f;

    printf("new_i is %d\n", new_i);
    printf("new_d is %f\n", new_d);
    printf("new_f is %f\n", new_f);

    while (1) {}

    return ret;
}

