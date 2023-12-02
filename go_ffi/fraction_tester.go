package main

/*
// This code block is a cgo preamble
#cgo LDFLAGS: -L. -lgo_ffi

#include <stdlib.h>
#include "go_ffi.h"

void print_cgo(const char *);
*/
import "C"
import (
	"fmt"
	"unsafe"
)

//export go_print
func go_print(argString *C.char) {
	fmt.Println(C.GoString(argString))
}

func main() {
	frac1 := C.Fraction{}
	frac1.numerator = 10
	frac1.denominator = 13
	frac1.str = C.CString("Hello")
	frac1.print_func = (*[0]byte)(C.print_cgo)
	frac2 := C.Fraction{}
	frac2.numerator = 9
	frac2.denominator = 17
	frac2.str = C.CString("World!")
	frac2.print_func = (*[0]byte)(C.print_cgo)

	retval := C.frac_mult(&frac1, &frac2)
	fmt.Printf("10/13 * 9/17 = %d/%d\n", frac1.numerator, frac1.denominator)
	fmt.Println("Error code = ", retval)
	C.free(unsafe.Pointer(frac1.str))
	C.free(unsafe.Pointer(frac2.str))
}
