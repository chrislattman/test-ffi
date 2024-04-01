package main

/*
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
} Fraction;

typedef void (*print_func_t)(const char *);

static void bridge_print_func(print_func_t f, const char *str) {
	f(str);
}
*/
import "C"
import "fmt"

//export fraction_multiply
func fraction_multiply(frac1 *C.Fraction, frac2 *C.Fraction) int32 {
	if frac1 != nil && frac2 != nil {
		numerator := frac1.numerator * frac2.numerator
		denominator := frac1.denominator * frac2.denominator
		frac1.numerator = numerator
		frac1.denominator = denominator
		C.bridge_print_func(C.print_func_t(frac1.print_func), frac1.str)
		C.bridge_print_func(C.print_func_t(frac2.print_func), frac2.str)
		fmt.Println("Finished with calculation!")
		return 0
	}
	return -1
}

func main() {

}
