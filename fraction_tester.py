import ctypes

PRINT_FUNC_TYPE = ctypes.CFUNCTYPE(None, ctypes.c_char_p)

# Mimics the structure in libfraction.c
class Fraction(ctypes.Structure):
    _fields_ = [("numerator", ctypes.c_int),
                ("denominator", ctypes.c_int),
                ("str", ctypes.c_char_p),
                ("print_func", PRINT_FUNC_TYPE)]

# The string argument is received by py_print_func as encoded bytes
def py_print_func(arg_string_bytes: bytes):
    print(arg_string_bytes.decode())

print_func = PRINT_FUNC_TYPE(py_print_func)

# Load shared library and set argument types and return type
libfraction = ctypes.CDLL("libfraction.so")

# Optional for our purposes, but necessary for variadic functions
# libfraction.fraction_multiply.argtypes = [ctypes.POINTER(Fraction), ctypes.POINTER(Fraction)]
# libfraction.fraction_multiply.restype = ctypes.c_int

frac1 = Fraction(10, 13, b"Hello", print_func)
frac2 = Fraction(9, 17, b"World!", print_func)
retval = libfraction.fraction_multiply(ctypes.pointer(frac1), ctypes.pointer(frac2))
print(f"10/13 * 9/17 = {frac1.numerator}/{frac1.denominator}")
print(f"Error code = {retval}")
