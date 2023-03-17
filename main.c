#include <stdio.h>
#include "lib_input.h"

int lib_func(struct lib_input *x);

int main(void)
{
    struct lib_input x;
    int i = 1;
    double d = 1.1;
    x.i_ptr = &i;
    x.d_ptr = &d;
    x.f = 1.2;

    //printf("Addr of ptr to struct %#lx\n");
    //printf("Addr of struct %#lx\n", &x);
    //printf("Addr of ptr to double %#lx\n", &x.d_ptr);
    //printf("Addr of double %#lx\n", &d);
    //printf("Addr of ptr to int %#lx\n", &x.i_ptr);
    //printf("Addr of int %#lx\n", &i);

    lib_func(&x);

    return 0;
}
