package java_ffi;

// Mimics the structure in libfraction.c
public class Fraction {
    int numerator, denominator;
    String str;

    void printFunc(String arg_string) {
        System.out.println(arg_string);
    }
}
