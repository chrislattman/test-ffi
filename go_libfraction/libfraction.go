package main

/*
#include <string.h>

typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
	unsigned char *in_bytes, *out_bytes;
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
		frac1inBytes := C.GoBytes(unsafe.Pointer(frac1.in_bytes), C.int(frac1.bytes_len))
		frac2inBytes := C.GoBytes(unsafe.Pointer(frac2.in_bytes), C.int(frac2.bytes_len))
		frac1outBytes := make([]byte, len(frac1inBytes))
		frac2outBytes := make([]byte, len(frac2inBytes))
		counter := uint64(0)
		for i, elem := range frac1inBytes {
			counter += uint64(elem)
			frac1outBytes[i] = elem ^ 0x5c
		}
		for i, elem := range frac2inBytes {
			counter += uint64(elem)
			frac2outBytes[i] = elem ^ 0x5c
		}
		// Assuming out_bytes has been allocated by the caller of this function!
		C.memcpy(unsafe.Pointer(frac1.out_bytes), unsafe.Pointer(&frac1outBytes[0]), frac1.bytes_len)
		C.memcpy(unsafe.Pointer(frac2.out_bytes), unsafe.Pointer(&frac2outBytes[0]), frac2.bytes_len)
		fmt.Println("The average of the bytes in frac1 and frac2 =", counter/uint64(len(frac1inBytes)+len(frac2inBytes)))
		fmt.Println("Finished with calculation!")
		return 0
	}
	return -1
}

func main() {}
