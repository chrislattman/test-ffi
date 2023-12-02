// This is the "glue code" that loads the libfraction shared library
#include <dlfcn.h>
#include <stdio.h>
#include "go_ffi.h"

int frac_mult(Fraction *f1, Fraction *f2) {
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);
    int retval;

#ifdef __APPLE__
    handle = dlopen("libfraction.dylib", RTLD_NOW);
#else
    handle = dlopen("libfraction.so", RTLD_NOW);
#endif
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    retval = fraction_multiply(f1, f2);
    dlclose(handle);
    return retval;
}
