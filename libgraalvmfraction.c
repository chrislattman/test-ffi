#include <stdlib.h>

#include "fraction.h"
#include "libgraalvmfraction.h"

int fraction_multiply(Fraction *frac1, Fraction *frac2) {
    graal_isolate_t *isolate = NULL;
    graal_isolatethread_t *thread = NULL;
    if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
        exit(1);
    }
    int res = graalvm_fraction_multiply(thread, frac1, frac2);
    graal_tear_down_isolate(thread);
    return res;
}
