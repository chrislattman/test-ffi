package main

// #cgo CFLAGS: -I${SRCDIR}/ffi_glue
// #cgo LDFLAGS: -L${SRCDIR}/ffi_glue -lgoffi
// void print(const char *);
// typedef void (*closure)();
// #include <stdlib.h>
// #include <go_ffi.h>
import "C"
import (
	"fmt"
	"unsafe"
)

// export print
func print(argString string) {
	fmt.Println(argString)
}

func main() {
	frac1 := C.Fraction{}
	frac1.numerator = 10
	frac1.denominator = 13
	frac1.str = C.CString("Hello")
	frac1.print_func = C.closure(C.print) // currently doesn't do anything
	frac2 := C.Fraction{}
	frac2.numerator = 9
	frac2.denominator = 17
	frac2.str = C.CString("World!")
	frac2.print_func = C.closure(C.print) // currently doesn't do anything

	retval := C.frac_mult(&frac1, &frac2)
	fmt.Printf("10/13 * 9/17 = %d/%d\n", frac1.numerator, frac1.denominator)
	fmt.Println("Error code = ", retval)
	C.free(unsafe.Pointer(frac1.str))
	C.free(unsafe.Pointer(frac2.str))
}
