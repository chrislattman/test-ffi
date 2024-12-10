#include <iostream>

#ifdef _WIN64
#define DllExport __declspec(dllexport)
#else
#define DllExport
#endif

using namespace std;

/**
 * @brief Fraction data type
 *
 */
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
} Fraction;

/**
 * @brief Multiplies two fractions together and stores the result in frac1.
 * Also prints out the string in each fraction structure.
 *
 * @param frac1
 * @param frac2
 * @return 0 on success, -1 on failure
 */
extern "C" DllExport int fraction_multiply(Fraction *frac1, Fraction *frac2) {
    if (frac1 != nullptr && frac2 != nullptr) {
        int numerator = frac1->numerator * frac2->numerator;
        int denominator = frac1->denominator * frac2->denominator;
        frac1->numerator = numerator;
        frac1->denominator = denominator;
        frac1->print_func(frac1->str);
        frac2->print_func(frac2->str);
        cout << "Finished with calculation!" << endl;
        return 0;
    }
    return -1;
}
