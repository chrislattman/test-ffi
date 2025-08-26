package main

/*
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
	unsigned char *bytes;
    size_t bytes_len;
} Fraction;

typedef void (*print_func_t)(const char *);

static void bridge_print_func(print_func_t f, const char *str) {
	f(str);
}
*/
import "C"
import (
	"fmt"
	"unsafe"
)

//export fraction_multiply
func fraction_multiply(frac1, frac2 *C.Fraction) int32 {
	if frac1 != nil && frac2 != nil {
		numerator := frac1.numerator * frac2.numerator
		denominator := frac1.denominator * frac2.denominator
		frac1.numerator = numerator
		frac1.denominator = denominator
		if frac1.print_func != nil && frac1.str != nil {
			C.bridge_print_func(frac1.print_func, frac1.str)
		}
		if frac2.print_func != nil && frac2.str != nil {
			C.bridge_print_func(frac2.print_func, frac2.str)
		}
		frac1Bytes := C.GoBytes(unsafe.Pointer(frac1.bytes), C.int(frac1.bytes_len))
		frac2Bytes := C.GoBytes(unsafe.Pointer(frac2.bytes), C.int(frac2.bytes_len))
		counter := uint64(0)
		for _, elem := range frac1Bytes {
			counter += uint64(elem)
		}
		for _, elem := range frac2Bytes {
			counter += uint64(elem)
		}
		fmt.Println("The average of the bytes in frac1 and frac2 =", counter/uint64(len(frac1Bytes)+len(frac2Bytes)))
		fmt.Println("Finished with calculation!")
		return 0
	}
	return -1
}

func main() {}
