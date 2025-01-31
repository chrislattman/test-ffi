using System.Runtime.InteropServices;

namespace FFI
{
    // Class that uses Platform Invoke (P/Invoke)
    // Also referred to by C# as "native interoperability"
    public class FractionTester // This needs to be public static partial class FractionTester if using LibraryImport
    {
        // Using DllImport instead of LibraryImport due to string marshaling
        [DllImport("fraction")]
        private static extern int fraction_multiply(ref Fraction frac1, ref Fraction frac2);

        public delegate void PrintFuncType(string argString);

        private static void CsPrintFunc(string argString)
        {
            Console.WriteLine(argString);
        }

        public static void Main() // optionally add string[] args like Java
        {
            Fraction frac1 = new()
            {
                numerator = 10,
                denominator = 13,
                str = "Hello",
                printFunc = CsPrintFunc
            };
            Fraction frac2 = new()
            {
                numerator = 9,
                denominator = 17,
                str = "World!",
                printFunc = CsPrintFunc
            };
            int retval = fraction_multiply(ref frac1, ref frac2);
            Console.WriteLine("10/13 * 9/17 = " + frac1.numerator + "/" + frac1.denominator);
            Console.WriteLine("Error code = " + retval);
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Fraction
    {
        public int numerator, denominator;
        public string str;
        public FractionTester.PrintFuncType printFunc;
    }
}
