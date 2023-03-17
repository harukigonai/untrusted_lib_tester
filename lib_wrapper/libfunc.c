#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "arg_struct.h"
#include "../lib_input.h"

int lib_func(int w, struct lib_input *x, double y, struct sub_input z)
{
    printf("entering lib_func\n");

    int ret;

    struct lib_enter_args args = {0};
    struct lib_enter_args *args_addr = &args;
    populate_arg(args_addr, w);
    populate_arg(args_addr, x);
    populate_arg(args_addr, y);
    populate_arg(args_addr, z);
    populate_ret(args_addr, ret);

    printf("Addr of w %#lx\n", &w);
    printf("Addr of ptr to struct %#lx\n", &x);
    printf("Addr of struct %#lx\n", x);
    printf("Addr of ptr to double %#lx\n", &x->d_ptr);
    printf("Addr of double %#lx\n", x->d_ptr);
    printf("Addr of ptr to int %#lx\n", &x->i_ptr);
    printf("Addr of int %#lx\n", x->i_ptr);
    printf("Addr of y %#lx\n", &y);
    printf("Addr of z %#lx\n", &z);


	int i = 0;
    // struct lib_input *
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(struct lib_input *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = 5;
    args.entity_metadata[i++] = 0;

	// struct lib_input
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(struct lib_input);
    args.entity_metadata[i++] = 4;
    args.entity_metadata[i++] = 16; // child 1 (int *) index
    args.entity_metadata[i++] = offsetof(struct lib_input, i_ptr); // child 1 (int *) offset;
    args.entity_metadata[i++] = ; // child 2 (double *) index;
    args.entity_metadata[i++] = offsetof(struct lib_input, d_ptr); // child 2 (double *) offset;
    args.entity_metadata[i++] = ; // child 4 (struct sub_input) index;
    args.entity_metadata[i++] = offsetof(struct lib_input, sub); // child 2 (double *) offset;
    args.entity_metadata[i++] = ; // child 5 (struct sub_input) index;
    args.entity_metadata[i++] = offsetof(struct lib_input, sub_ptr); // child 2 (double *) offset;

	// lib_input.i_ptr (int *)
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(int *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = ; // child 1 (int)
    args.entity_metadata[i++] = 0;

	// *(lib_input.i_ptr)
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(int);
    args.entity_metadata[i++] = 0;
    
	// lib_input.d_ptr
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(double *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = ; // child 1 (double)
    args.entity_metadata[i++] = 0;

	// *(lib_input.d_ptr)
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(double);
    args.entity_metadata[i++] = 0;

	// sub_input *
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(struct sub_input *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = ;
    args.entity_metadata[i++] = 0;

	// struct sub_input
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(struct sub_input);
    args.entity_metadata[i++] = 2;
    args.entity_metadata[i++] = ; // child 1 s[10]
    args.entity_metadata[i++] = offsetof(struct sub_input, s); // child 1 (int *) offset;
    args.entity_metadata[i++] = ; // child 2 (float *)
    args.entity_metadata[i++] = offsetof(struct sub_input, f_sub); // child 2 (double *) offset;

	// sub_input.s
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(char[10]);
    args.entity_metadata[i++] = 0;
	
	// sub_input.f_sub
	args.entity_metadata[i++] = 1;
	args.entity_metadata[i++] = sizeof(float *);
	args.entity_metadata[i++] = 1;
	args.entity_metadata[i++] = ;
	args.entity_metadata[i++] = 0;

	// float
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(float);
    args.entity_metadata[i++] = 0;

    args.entity_metadata_size = i;
 
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

