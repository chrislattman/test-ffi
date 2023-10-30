#include <stdlib.h>

/**
 * @brief Fraction data type
 *
 */
typedef struct fraction {
    int numerator, denominator;
} Fraction;

/**
 * @brief Multiplies two fractions together and stores the result in frac1.
 *
 * @param frac1
 * @param frac2
 * @return 0 on success, -1 on failure
 */
int fraction_multiply(Fraction *frac1, Fraction *frac2)
{
    if (frac1 != NULL && frac2 != NULL) {
        int numerator = frac1->numerator * frac2->numerator;
        int denominator = frac1->denominator * frac2->denominator;
        frac1->numerator = numerator;
        frac1->denominator = denominator;
        return 0;
    }
    return -1;
}
