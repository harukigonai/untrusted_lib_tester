#include <stdint.h>
#define _GNU_SOURCE

#include "../lib_input.h"
#include "arg_struct.h"
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

struct lib_output actual_lib_func(int w, struct lib_input *x, double y,
                                  struct sub_input z) {
    *x->i_ptr = 1000;
    *x->d_ptr = 1100.0;
    x->f = 2200.0;
    strcpy(x->sub.s, "test 2");

    struct lib_output out = {0};
    out.i = 100;
    out.i_ptr = mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    *out.i_ptr = 1234;
    out.d_ptr = mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    *out.d_ptr = 1234.5;
    out.s_ptr = "a";
    strcpy(out.s, "yeah");
    return out;
}

/* Pretend this is the wrapper */
struct lib_output lib_func(int w, struct lib_input *x, double y,
                           struct sub_input z) {
    printf("entering lib_func\n");

    struct lib_output ret;

    struct lib_enter_args args = {0};
    struct lib_enter_args *args_addr = &args;
    populate_arg(args_addr, w);
    populate_arg(args_addr, x);
    populate_arg(args_addr, y);
    populate_arg(args_addr, z);
    populate_ret(args_addr, ret);

    /*
    printf("Addr of w %#lx\n", (uint64_t)&w);
    printf("Addr of ptr to struct %#lx\n", (uint64_t)&x);
    printf("Addr of struct %#lx\n", (uint64_t)x);
    printf("Addr of ptr to double %#lx\n", (uint64_t)&x->d_ptr);
    printf("Addr of double %#lx\n", (uint64_t)x->d_ptr);
    printf("Addr of ptr to int %#lx\n", (uint64_t)&x->i_ptr);
    printf("Addr of int %#lx\n", (uint64_t)x->i_ptr);
    printf("Addr of sub %#lx\n", (uint64_t)&x->sub);
    printf("Value of sub_ptr %#lx\n", (uint64_t)x->sub_ptr);
    printf("Value of sub.f_sub %#lx\n", (uint64_t)x->sub.f_sub);
    printf("Addr of int %#lx\n", (uint64_t)x->i_ptr);
    printf("Addr of y %#lx\n", (uint64_t)&y);
    printf("Addr of z %#lx\n", (uint64_t)&z);
    printf("Value of z.f_sub %#lx\n", (uint64_t)z.f_sub);
    printf("Value of z.self %#lx\n", (uint64_t)z.self);
    printf("new_w: %d\n", w);
    */
    printf("\n---\n\n");
    printf("w: %d\n", w);
    printf("x: %#lx\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ f: %f\n"
           " \\_ sub\n"
           " |   \\_ s: %s\n"
           " |   \\_ f_sub: %#lx\n"
           " |   \\_ self: %#lx\n"
           " \\_ sub_ptr: %#lx\n",
           x, x->i_ptr, *(x->i_ptr), x->d_ptr, *(x->d_ptr), x->f, x->sub.s,
           x->sub.f_sub, x->sub.self, x->sub_ptr);
    printf("y: %ld\n", y);
    printf("z:\n"
           " \\_ s: %s\n"
           " \\_ f_sub: %#lx\n"
           " |   \\_ *f_sub: %f\n"
           " \\_ self: %#lx\n",
           z.s, z.f_sub, *(z.f_sub), z.self);
    printf("addr of ret: %#lx\n", &ret);

    int i = 0;
    // struct lib_input * [0]
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(struct lib_input *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = 5;
    args.entity_metadata[i++] = 0;

    // struct lib_input [5]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(struct lib_input);
    args.entity_metadata[i++] = 4;
    args.entity_metadata[i++] = 16; // child 1 (int *) index
    args.entity_metadata[i++] =
        offsetof(struct lib_input, i_ptr); // child 1 (int *) offset;
    args.entity_metadata[i++] = 24;        // child 2 (double *) index;
    args.entity_metadata[i++] =
        offsetof(struct lib_input, d_ptr); // child 2 (double *) offset;
    args.entity_metadata[i++] = 37;        // child 4 (struct sub_input) index;
    args.entity_metadata[i++] =
        offsetof(struct lib_input, sub); // child 2 (double *) offset;
    args.entity_metadata[i++] = 32;      // child 5 (struct sub_input *) index;
    args.entity_metadata[i++] =
        offsetof(struct lib_input, sub_ptr); // child 2 (double *) offset;

    // lib_input.i_ptr (int *) [16]
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(int *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = 21; // child 1 (int)
    args.entity_metadata[i++] = 0;

    // *(lib_input.i_ptr) [21]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(int);
    args.entity_metadata[i++] = 0;

    // lib_input.d_ptr [24]
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(double *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = 29; // child 1 (double)
    args.entity_metadata[i++] = 0;

    // *(lib_input.d_ptr) [29]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(double);
    args.entity_metadata[i++] = 0;

    // sub_input * [32]
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(struct sub_input *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = 37; // struct sub_input
    args.entity_metadata[i++] = 0;

    // struct sub_input [37]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(struct sub_input);
    args.entity_metadata[i++] = 3;
    args.entity_metadata[i++] = 44; // child 1 s[10]
    args.entity_metadata[i++] =
        offsetof(struct sub_input, s); // child 1 (int *) offset;
    args.entity_metadata[i++] = 49;    // child 2 (float *)
    args.entity_metadata[i++] =
        offsetof(struct sub_input, f_sub); // child 2 (double *) offset;
    args.entity_metadata[i++] = 32;        // child 2 (float *)
    args.entity_metadata[i++] =
        offsetof(struct sub_input, self); // child 2 (double *) offset;

    // sub_input.s [46]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(char[10]);
    args.entity_metadata[i++] = 0;

    // sub_input.f_sub [49]
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = sizeof(float *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = 54;
    args.entity_metadata[i++] = 0;

    // float [54]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(float);
    args.entity_metadata[i++] = 0;

    // struct lib_output [57]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(struct lib_output);
    args.entity_metadata[i++] = 3;
    args.entity_metadata[i++] = 16; // child 1 (int *) index
    args.entity_metadata[i++] =
        offsetof(struct lib_output, i_ptr); // child 1 (int *) offset;
    args.entity_metadata[i++] = 24;        // child 2 (double *) index;
    args.entity_metadata[i++] =
        offsetof(struct lib_output, d_ptr); // child 2 (double *) offset;
    args.entity_metadata[i++] = 66;        // child 3 (char *) index;
    args.entity_metadata[i++] =
        offsetof(struct lib_output, s_ptr); // child 3 (char *) offset;

    // char * [66]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(char *);
    args.entity_metadata[i++] = 1;
    args.entity_metadata[i++] = 71; // child 1 (char) index
    args.entity_metadata[i++] = 0;  // child 1 (char) offset

    // char [71]
    args.entity_metadata[i++] = 0;
    args.entity_metadata[i++] = sizeof(char);
    args.entity_metadata[i++] = 0;

    args.entity_metadata_size = i;
    printf("\n---\n\n");
    printf("We used %d slots in metadata\n", i);

    args.arg_entity_index[0] = 21;
    args.arg_entity_index[1] = 0;
    args.arg_entity_index[2] = 29;
    args.arg_entity_index[3] = 37;
    args.ret_entity_index = 57;

    printf("Old i_addr: %#lx\n", (uint64_t)x->i_ptr);
    printf("Old d_addr: %#lx\n", (uint64_t)x->d_ptr);

    // invoke syscall 888
    struct lib_enter_args *new_args = (void *)syscall(888, args_addr);

    int num_args = new_args->num_args;
    assert(num_args == 4);

    int new_w = *((int *)new_args->args[0]);
    struct lib_input *new_x = *((struct lib_input **)new_args->args[1]);
    double new_y = *((double *)new_args->args[2]);
    struct sub_input new_z = *((struct sub_input *)new_args->args[3]);

    // Note that the ret val type is struct lib_output
    struct lib_output *new_ret_ptr = ((struct lib_output *)new_args->ret);

    printf("\n---\n\n");
    printf("new_w: %d\n", new_w);
    printf("new_x: %#lx\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ f: %f\n"
           " \\_ sub\n"
           " |   \\_ s: %s\n"
           " |   \\_ f_sub: %#lx\n"
           " |   \\_ self: %#lx\n"
           " \\_ sub_ptr: %#lx\n",
           new_x, new_x->i_ptr, *(new_x->i_ptr), new_x->d_ptr, *(new_x->d_ptr),
           new_x->f, new_x->sub.s, new_x->sub.f_sub, new_x->sub.self,
           new_x->sub_ptr);
    printf("new_y: %ld\n", new_y);
    printf("new_z:\n"
           " \\_ s: %s\n"
           " \\_ f_sub: %#lx\n"
           " |   \\_ *f_sub: %f\n"
           " \\_ self: %#lx\n",
           new_z.s, new_z.f_sub, *(new_z.f_sub), new_z.self);
    printf("new addr of ret: %#lx\n", new_ret_ptr);

    *new_ret_ptr = actual_lib_func(new_w, new_x, new_y, new_z);

    printf("actual_lib_func is updating some of the stuff...\n\n");

    printf("\n---\n\n");
    printf("new_w: %d\n", new_w);
    printf("new_x: %#lx\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ f: %f\n"
           " \\_ sub\n"
           " |   \\_ s: %s\n"
           " |   \\_ f_sub: %#lx\n"
           " |   \\_ self: %#lx\n"
           " \\_ sub_ptr: %#lx\n",
           new_x, new_x->i_ptr, *(new_x->i_ptr), new_x->d_ptr, *(new_x->d_ptr),
           new_x->f, new_x->sub.s, new_x->sub.f_sub, new_x->sub.self,
           new_x->sub_ptr);
    printf("new_y: %ld\n", new_y);
    printf("new_z:\n"
           " \\_ s: %s\n"
           " \\_ f_sub: %#lx\n"
           " |   \\_ *f_sub: %f\n"
           " \\_ self: %#lx\n",
           new_z.s, new_z.f_sub, *(new_z.f_sub), new_z.self);
    printf("new_ret: %#lx\n"
           " \\_ i: %d\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ s_ptr: %s\n"
           " \\_ s: %s\n",
           new_ret_ptr, new_ret_ptr->i, new_ret_ptr->i_ptr,
           *new_ret_ptr->i_ptr, new_ret_ptr->d_ptr, *new_ret_ptr->d_ptr,
           new_ret_ptr->s_ptr, new_ret_ptr->s);
    // Copies contents of new_args->ret into args_addr.ret
    syscall(889);

    printf("We exited 889. ret.i is %d\n\n", ret.i);

    printf("\n---\n\n");
    printf("w: %d\n", w);
    printf("x: %#lx\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ f: %f\n"
           " \\_ sub\n"
           " |   \\_ s: %s\n"
           " |   \\_ f_sub: %#lx\n"
           " |   \\_ self: %#lx\n"
           " \\_ sub_ptr: %#lx\n",
           x, x->i_ptr, *(x->i_ptr), x->d_ptr, *(x->d_ptr), x->f, x->sub.s,
           x->sub.f_sub, x->sub.self, x->sub_ptr);
    printf("y: %ld\n", y);
    printf("z:\n"
           " \\_ s: %s\n"
           " \\_ f_sub: %#lx\n"
           " |   \\_ *f_sub: %f\n"
           " \\_ self: %#lx\n",
           z.s, z.f_sub, *(z.f_sub), z.self);
    printf("ret: %#lx\n"
           " \\_ i: %d\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ s_ptr: %s\n"
           " \\_ s: %s\n",
           &ret, ret.i, ret.i_ptr, *ret.i_ptr, ret.d_ptr, *ret.d_ptr,
           ret.s_ptr, ret.s);

    return ret;
}
