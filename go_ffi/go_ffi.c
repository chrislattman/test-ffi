// This is the "glue code" that loads the libfraction shared library
// This code (including struct fraction) could have been placed in the cgo
// preamble in fraction_tester.go, but I chose to place it in a separate file
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "go_ffi.h"

int frac_mult(Fraction *f1, Fraction *f2) {
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);
    int retval;
    char *cwd = NULL, *libpath = NULL;
    const char *libname;
    size_t dirlen;

    // uses malloc to allocate a buffer with exactly the right size
    cwd = getcwd(NULL, 0);
    if (cwd == NULL) {
        return -1;
    }
    dirlen = strlen(cwd);
#ifdef __APPLE__
    libname = "/libfraction.dylib";
#else
    libname = "/libfraction.so";
#endif
    libpath = calloc(dirlen + strlen(libname), sizeof(char));
    if (libpath == NULL) {
        free(cwd);
        return -1;
    }
    strcat(libpath, cwd);
    strcat(libpath + dirlen, libname);
    handle = dlopen(libpath, RTLD_NOW);
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    retval = fraction_multiply(f1, f2);
    dlclose(handle);
    free(libpath);
    free(cwd);
    return retval;
}
