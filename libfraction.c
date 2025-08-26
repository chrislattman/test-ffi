#include <stdio.h>

/**
 * @brief Fraction data type
 *
 */
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
    unsigned char *bytes;
    size_t bytes_len;
} Fraction;

/**
 * @brief Multiplies two fractions together and stores the result in frac1.
 * Also prints out the string in each fraction structure.
 *
 * @param frac1
 * @param frac2
 * @return 0 on success, -1 on failure
 */
int fraction_multiply(Fraction *frac1, Fraction *frac2) {
    if (frac1 != NULL && frac2 != NULL) {
        int numerator = frac1->numerator * frac2->numerator;
        int denominator = frac1->denominator * frac2->denominator;
        frac1->numerator = numerator;
        frac1->denominator = denominator;
        if (frac1->print_func != NULL && frac1->str != NULL) {
            frac1->print_func(frac1->str);
        }
        if (frac2->print_func != NULL && frac2->str != NULL) {
            frac2->print_func(frac2->str);
        }
        size_t counter = 0;
        for (size_t i = 0; i < frac1->bytes_len; i++) {
            counter += frac1->bytes[i];
        }
        for (size_t i = 0; i < frac2->bytes_len; i++) {
            counter += frac2->bytes[i];
        }
        printf("The average of the bytes in frac1 and frac2 = %lu\n", counter / (frac1->bytes_len + frac2->bytes_len));
        printf("Finished with calculation!\n");
        return 0;
    }
    return -1;
}
