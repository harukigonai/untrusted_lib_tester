#ifndef __ARG_STRUCT_H
#define __ARG_STRUCT_H

// TODO: make this an actual kernel uapi header

#include <stdint.h>
#include <sys/types.h>

#define populate_arg(args_addr, arg) {\
        args_addr->args[args_addr->num_args] = &arg;\
        args_addr->args_size[args_addr->num_args] = sizeof(arg);\
        args_addr->args_is_output_buf[args_addr->num_args] = 1;\
        args_addr->num_args++;\
    }

#define populate_ret(args_addr, arg) {\
        args_addr->ret_val = &arg;\
        args_addr->ret_val_size = sizeof(arg);\
    }

#define MAX_ARGS 100

struct lib_enter_args {
    int num_args;
    void *args[MAX_ARGS];
    size_t args_size[MAX_ARGS];
    int args_is_output_buf[MAX_ARGS];

    uint64_t entity_metadata[500];
    size_t entity_metadata_size;
    int arg_entity_index[MAX_ARGS];

    void *ret_val;
    size_t ret_val_size;
};

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#endif
