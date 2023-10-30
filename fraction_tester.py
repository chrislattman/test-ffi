import ctypes

class Fraction(ctypes.Structure):
    _fields_ = [("numerator", ctypes.c_int),
                ("denominator", ctypes.c_int),
                ("str", ctypes.c_char_p)]

# Load shared library and set argument types and return type
libfraction = ctypes.CDLL("libfraction.so")
# libfraction.fraction_multiply.argtypes = [ctypes.POINTER(Fraction), ctypes.POINTER(Fraction)]
# libfraction.fraction_multiply.restype = ctypes.c_int

frac1 = Fraction(10, 13, b"Hello")
frac2 = Fraction(9, 17, b"World!")
retval = libfraction.fraction_multiply(ctypes.pointer(frac1), ctypes.pointer(frac2))
print(f"10/13 * 9/17 = {frac1.numerator}/{frac1.denominator}")
print(f"Error code = {retval}")
