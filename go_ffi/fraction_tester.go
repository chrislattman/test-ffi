package main

// The multi-line comment below is a cgo preamble

/*
// This links the code in go_ffi.c, which is compiled into a separate library
// prior to building the Go project
#cgo LDFLAGS: -L. -lgo_ffi

#include <stdlib.h>
#include <string.h>
#include "go_ffi.h"

void print_cgo(const char *);
*/
import "C"
import (
	"encoding/hex"
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
	buf1 := []byte("somedata")
	frac1.bytes_len = C.size_t(len(buf1))
	frac1.in_bytes = (*C.uchar)(C.malloc(frac1.bytes_len))
	frac1.out_bytes = (*C.uchar)(C.malloc(frac1.bytes_len))
	C.memcpy(unsafe.Pointer(frac1.in_bytes), unsafe.Pointer(&buf1[0]), frac1.bytes_len)
	frac2 := C.Fraction{}
	frac2.numerator = 9
	frac2.denominator = 17
	frac2.str = C.CString("World!")
	frac2.print_func = (*[0]byte)(C.print_cgo)
	buf2 := []byte("somemoredata")
	frac2.bytes_len = C.size_t(len(buf2))
	frac2.in_bytes = (*C.uchar)(C.malloc(frac2.bytes_len))
	frac2.out_bytes = (*C.uchar)(C.malloc(frac2.bytes_len))
	C.memcpy(unsafe.Pointer(frac2.in_bytes), unsafe.Pointer(&buf2[0]), frac2.bytes_len)

	retval := C.frac_mult(&frac1, &frac2)
	fmt.Printf("10/13 * 9/17 = %d/%d\n", frac1.numerator, frac1.denominator)
	frac1Result := C.GoBytes(unsafe.Pointer(frac1.out_bytes), C.int(frac1.bytes_len))
	frac2Result := C.GoBytes(unsafe.Pointer(frac2.out_bytes), C.int(frac2.bytes_len))
	fmt.Printf("b'somedata' XOR 0x5c = 0x%s\n", hex.EncodeToString(frac1Result))
	fmt.Printf("b'somemoredata' XOR 0x5c = 0x%s\n", hex.EncodeToString(frac2Result))
	fmt.Println("Error code =", retval)
	C.free(unsafe.Pointer(frac1.in_bytes))
	C.free(unsafe.Pointer(frac2.in_bytes))
	C.free(unsafe.Pointer(frac1.out_bytes))
	C.free(unsafe.Pointer(frac2.out_bytes))
	C.free(unsafe.Pointer(frac1.str))
	C.free(unsafe.Pointer(frac2.str))
}
