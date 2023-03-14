#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "arg_struct.h"
#include "../lib_input.h"

int lib_func(struct lib_input *x)
{
    printf("entering lib_func\n");

    int ret;

    struct lib_enter_args args = {0};
    struct lib_enter_args *args_addr = &args;
    populate_arg(args_addr, x);
    populate_ret(args_addr, ret);

    // struct lib_input *
    args.entity_metadata[0] = 0;
    args.entity_metadata[1] = sizeof(struct lib_input *);
    args.entity_metadata[2] = 1;
    args.entity_metadata[3] = 5;
    args.entity_metadata[4] = 0;

    args.entity_metadata[5] = 1;
    args.entity_metadata[6] = sizeof(struct lib_input);
    args.entity_metadata[7] = 2;
    args.entity_metadata[8] = 12; // child 1 (int *) index
    args.entity_metadata[9] = offsetof(struct lib_input, i_ptr); // child 1 (int *) offset;
    args.entity_metadata[10] = 20; // child 2 (double *) index;
    args.entity_metadata[11] = offsetof(struct lib_input, d_ptr); // child 2 (double *) offset;

    args.entity_metadata[12] = 2;
    args.entity_metadata[13] = sizeof(int *);
    args.entity_metadata[14] = 1;
    args.entity_metadata[15] = 17; // child 1 (int)
    args.entity_metadata[16] = 0;

    args.entity_metadata[17] = 3;
    args.entity_metadata[18] = sizeof(int);
    args.entity_metadata[19] = 0;
    
    args.entity_metadata[20] = 4;
    args.entity_metadata[21] = sizeof(double *);
    args.entity_metadata[22] = 1;
    args.entity_metadata[23] = 25; // child 1 (double)
    args.entity_metadata[24] = 0;

    args.entity_metadata[25] = 5;
    args.entity_metadata[26] = sizeof(double);
    args.entity_metadata[27] = 0;

    args.entity_metadata_size = 26;
    args.arg_entity_index[0] = 0;

    printf("Old i_addr: %#lx\n", x->i_ptr);
    printf("Old d_addr: %#lx\n", x->d_ptr);

    // invoke syscall 888
    struct lib_enter_args *new_args = syscall(888, args_addr);

    int num_args = new_args->num_args;
    struct lib_input *new_x = *((struct lib_input **)new_args->args[0]);

    int *new_i_ptr = new_x->i_ptr;
    int new_i = *new_i_ptr;
    double *new_d_ptr = new_x->d_ptr;
    double new_d = *new_d_ptr;
    float new_f = new_x->f;

    printf("New i_addr: %#lx\n", new_x->i_ptr);
    printf("New d addr: %#lx\n", new_x->d_ptr);

    printf("new_i is %d\n", new_i);
    printf("new_d is %f\n", new_d);
    printf("new_f is %f\n", new_f);

    while (1) {}

    return ret;
}

