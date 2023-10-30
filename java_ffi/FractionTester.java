package java_ffi;

// Class that uses the Java Native Interface (JNI)
public class FractionTester {
    // Needed to use JNI
    static {
        System.loadLibrary("native");
    }

    public static void main(String[] args) {
        FractionTester fractionTester = new FractionTester();
        Fraction frac1 = new Fraction();
        Fraction frac2 = new Fraction();
        frac1.numerator = 10;
        frac1.denominator = 13;
        frac2.numerator = 9;
        frac2.denominator = 17;
        int retval = fractionTester.fractionMultiply(frac1, frac2);
        System.out.printf("%d/%d * %d/%d = %d/%d\n", 10, 13, 9, 17, frac1.numerator, frac1.denominator);
        System.out.println("Return value = " + retval);
    }

    // Maps to Java_java_1ffi_FractionTester_fractionMultiply
    private native int fractionMultiply(Fraction frac1, Fraction frac2);
}
