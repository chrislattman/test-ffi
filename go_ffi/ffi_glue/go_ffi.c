#include <dlfcn.h>
#include "go_ffi.h"

int frac_mult(Fraction *f1, Fraction *f2) {
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);
    int retval;

    handle = dlopen("../../libfraction.so", RTLD_NOW);
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    retval = fraction_multiply(f1, f2);
    dlclose(handle);
    return retval;
}
