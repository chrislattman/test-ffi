#pragma once

// Mimics the structure in libfraction.c
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
} Fraction;

int frac_mult(Fraction *, Fraction *);
