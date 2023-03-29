#define _GNU_SOURCE

#include "../lib_input.h"
#include "arg_struct.h"
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/syscall.h>

void real_func_1(void)
{
    struct lib_enter_args args = {
    	.num_args = 0,
	.entity_metadata = {},
	.arg_entity_index = { -1 },
	.ret_entity_index = -1,
    };

    struct lib_enter_args *new_args = syscall(888, &args);

    void (*orig)(void);
    orig = dlsym(RTLD_NEXT, "real_func_1");
    (*orig)();

    syscall(889);

}

void real_func_2(void)
{
	struct lib_enter_args args = {
		.num_args = 0,
		.entity_metadata = {},
		.arg_entity_index = { -1 },
		.ret_entity_index = -1,
	};

	struct lib_enter_args *new_args = syscall(888, &args);

	void (*orig)(void);
	orig = dlsym(RTLD_NEXT, "real_func_1");
	(*orig)();
        syscall(889);
}

