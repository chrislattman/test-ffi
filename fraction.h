// This file exists because GraalVM currently cannot autogenerate C header files
// from Java interfaces with @CStruct annotations (e.g. Fraction.java)
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
} Fraction;
