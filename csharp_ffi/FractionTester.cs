using System.Runtime.InteropServices;
using System.Text;

namespace FFI
{
    // Class that uses Platform Invoke (P/Invoke)
    // Also referred to by C# as "native interoperability"
    public class FractionTester // This needs to be public static partial class FractionTester if using LibraryImport
    {
        // Using DllImport instead of LibraryImport due to string marshaling in Structs
        // If using LibraryImport on a Unix system and passing strings:
        // [LibraryImport("fraction", StringMarshalling = StringMarshalling.Utf8)]
        // In C# (and on Windows), strings are encoded as UTF-16 (wide chars)
        [DllImport("fraction")]
        private static extern int fraction_multiply(ref Fraction frac1, ref Fraction frac2);

        public delegate void PrintFuncType(string argString);

        private static void CsPrintFunc(string argString)
        {
            Console.WriteLine(argString);
        }

        public static void Main() // optionally add string[] args like Java
        {
            byte[] frac1Bytes = Encoding.UTF8.GetBytes("somedata");
            byte[] frac2Bytes = Encoding.UTF8.GetBytes("somemoredata");
            Fraction frac1 = new()
            {
                numerator = 10,
                denominator = 13,
                str = "Hello",
                printFunc = CsPrintFunc,
                bytes = Marshal.AllocHGlobal(frac1Bytes.Length),
                bytesLen = (UIntPtr)frac1Bytes.Length
            };
            Fraction frac2 = new()
            {
                numerator = 9,
                denominator = 17,
                str = "World!",
                printFunc = CsPrintFunc,
                bytes = Marshal.AllocHGlobal(frac2Bytes.Length),
                bytesLen = (UIntPtr)frac2Bytes.Length
            };
            Marshal.Copy(frac1Bytes, 0, frac1.bytes, frac1Bytes.Length);
            Marshal.Copy(frac2Bytes, 0, frac2.bytes, frac2Bytes.Length);
            int retval = fraction_multiply(ref frac1, ref frac2);
            Console.WriteLine("10/13 * 9/17 = " + frac1.numerator + "/" + frac1.denominator);
            Console.WriteLine("Error code = " + retval);
            Marshal.FreeHGlobal(frac1.bytes);
            Marshal.FreeHGlobal(frac2.bytes);
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Fraction
    {
        public int numerator, denominator;
        public string str;
        public FractionTester.PrintFuncType printFunc;
        public IntPtr bytes;
        public UIntPtr bytesLen;
    }
}
