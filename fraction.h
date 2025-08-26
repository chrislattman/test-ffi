// This file exists because GraalVM currently cannot autogenerate C header files
// from Java interfaces with @CStruct annotations (e.g. LibFraction.java)
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
    unsigned char *bytes;
    size_t bytes_len;
} Fraction;
