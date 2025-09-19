package java_ffi;

import java.nio.charset.StandardCharsets;

// Class that uses the Java Native Interface (JNI)
public class FractionTester {
    static {
        // Loads the libjava_ffi library
        System.loadLibrary("java_ffi");
    }

    static class Fraction {
        int numerator, denominator;
        String str;
        byte[] inBytes, outBytes;

        void printFunc(String argString) {
            System.out.println(argString);
        }
    }

    // "Abstract" function which maps to Java_java_1ffi_FractionTester_fractionMultiply
    private native int fractionMultiply(Fraction frac1, Fraction frac2);

    public static void main(String[] args) {
        Fraction frac1 = new Fraction();
        Fraction frac2 = new Fraction();
        frac1.numerator = 10;
        frac1.denominator = 13;
        frac1.str = "Hello";
        frac1.inBytes = "somedata".getBytes(StandardCharsets.UTF_8);
        frac1.outBytes = new byte[frac1.inBytes.length];
        frac2.numerator = 9;
        frac2.denominator = 17;
        frac2.str = "World!";
        frac2.inBytes = "somemoredata".getBytes(StandardCharsets.UTF_8);
        frac2.outBytes = new byte[frac2.inBytes.length];
        FractionTester fractionTester = new FractionTester();
        int retval = fractionTester.fractionMultiply(frac1, frac2);
        System.out.printf("10/13 * 9/17 = %d/%d\n", frac1.numerator, frac1.denominator);
        System.out.print("b'somedata' XOR 0x5c = 0x");
        for (int i = 0; i < frac1.outBytes.length; i++) {
            System.out.printf("%02x", frac1.outBytes[i]);
        }
        System.out.println();
        System.out.print("b'somemoredata' XOR 0x5c = 0x");
        for (int i = 0; i < frac2.outBytes.length; i++) {
            System.out.printf("%02x", frac2.outBytes[i]);
        }
        System.out.println();
        System.out.println("Error code = " + retval);
    }
}
