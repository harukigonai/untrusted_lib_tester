#include <stdint.h>
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

void real_func_2(void)
{
	int a = 0;
	a = 2;
	for (int i = 0; i < 312312; i++) {
		a *= 3;
		if (a % 2 == 0)
			a -= 1;
	}
}

void real_func_1(void)
{
	for (int j = 0; j < 312312; j++) {
		j += 1;
	}
	real_func_2();
}

