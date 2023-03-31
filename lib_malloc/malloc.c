#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

static int setting_up_untrusted_malloc = 0;

static void *(*trusted_malloc_ptr)(size_t) = NULL;
static void *(*untrusted_malloc_ptr)(size_t) = NULL;

void trusted_malloc_init()
{
    trusted_malloc_ptr = dlsym(RTLD_NEXT, "malloc");
}

void untrusted_malloc_init()
{
    setting_up_untrusted_malloc = 1;
    void *new_libc = dlmopen(LM_ID_NEWLM, "/lib/aarch64-linux-gnu/libc.so.6", RTLD_NOW);
    untrusted_malloc_ptr = dlsym(new_libc, "malloc");
    setting_up_untrusted_malloc = 0;
}

void *malloc(size_t size)
{
    if (!trusted_malloc_ptr) {
        trusted_malloc_init();
    } else if (!untrusted_malloc_ptr && !setting_up_untrusted_malloc) {
        untrusted_malloc_init();
    }

    void *res;
    if (syscall(890) && !setting_up_untrusted_malloc) {
        res = untrusted_malloc_ptr(size);
    } else {
        res = trusted_malloc_ptr(size);
    }

    return res;
}
