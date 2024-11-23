// This "glue code" is needed because WebAssembly code cannot access the file system,
// since the code is running in a sandboxed web browser environment
#include <emscripten/emscripten.h>

// Mimics the structure in libfraction.c
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
} Fraction;

int fraction_multiply(Fraction *frac1, Fraction *frac2);

EM_JS(void, js_print_function, (const char *arg), {
    Module.js_print(UTF8ToString(arg));
});

EMSCRIPTEN_KEEPALIVE int frac_mult(int frac1Numerator, int frac1Denominator, const char *frac1Str,
        int frac2Numerator, int frac2Denominator, const char *frac2Str) {
    Fraction f1, f2;
    int retval;

    f1 = (Fraction) {
        .numerator = frac1Numerator,
        .denominator = frac1Denominator,
        .str = frac1Str,
        .print_func = js_print_function
    };
    f2 = (Fraction) {
        .numerator = frac2Numerator,
        .denominator = frac2Denominator,
        .str = frac2Str,
        .print_func = js_print_function
    };
    retval = fraction_multiply(&f1, &f2);
    return retval;
}
