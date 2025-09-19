#pragma once

// Mimics the structure in libfraction.c
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
    unsigned char *in_bytes, *out_bytes;
    size_t bytes_len;
} Fraction;

int frac_mult(Fraction *, Fraction *);
