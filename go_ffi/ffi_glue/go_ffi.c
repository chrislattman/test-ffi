// This is the "glue code" that calls libfraction.so
#include <dlfcn.h>
#include <stdio.h>
#include "go_ffi.h"

void print(const char *str) {
    printf("%s\n", str);
}

int frac_mult(Fraction *f1, Fraction *f2) {
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);
    int retval;

    handle = dlopen("../../libfraction.so", RTLD_NOW);
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    f1->print_func = print;
    f2->print_func = print;
    retval = fraction_multiply(f1, f2);
    dlclose(handle);
    return retval;
}
