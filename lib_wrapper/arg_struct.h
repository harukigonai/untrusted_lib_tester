#ifndef __ARG_STRUCT_H
#define __ARG_STRUCT_H

// TODO: make this an actual kernel uapi header

#include <stdint.h>
#include <sys/types.h>

#define populate_arg(args_addr, arg) {\
        args_addr->args[args_addr->num_args] = &arg;\
        args_addr->num_args++;\
    }

#define populate_ret(args_addr, arg) {\
        args_addr->ret = &arg;\
    }

#define MAX_ARGS 100

struct lib_enter_args {
    int num_args;
    void *args[MAX_ARGS];
    int arg_entity_index[MAX_ARGS];

    void *ret;
    int ret_entity_index;

    uint64_t entity_metadata[4096];
    size_t entity_metadata_size;
};

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#endif
