#include <stdio.h>
#include <math.h>
#include "lib_input.h"

int main(void)
{
    struct lib_input x;
    int i = 1;
    double d = 1.1;
    x.i_ptr = &i;
    x.d_ptr = &d;
    x.f = 1.2;

    lib_func(&x);

    return 0;
}