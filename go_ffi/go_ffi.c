// This is the "glue code" that loads the libfraction shared library
// This code (including struct fraction) could have been placed in the cgo
// preamble in fraction_tester.go, but I chose to place it in a separate file
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "go_ffi.h"

int frac_mult(Fraction *f1, Fraction *f2) {
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);
    int retval;
    char libpath[200];
    size_t dirlen;

    getcwd(libpath, sizeof(libpath) - 1);
    dirlen = strlen(libpath);
#ifdef __APPLE__
    strncat(libpath + dirlen, "/libfraction.dylib", sizeof(libpath) - dirlen - 1);
#else
    strncat(libpath + dirlen, "/libfraction.so", sizeof(libpath) - dirlen - 1);
#endif
    handle = dlopen(libpath, RTLD_NOW);
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    retval = fraction_multiply(f1, f2);
    dlclose(handle);
    return retval;
}
