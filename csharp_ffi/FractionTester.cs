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
                inBytes = Marshal.AllocHGlobal(frac1Bytes.Length),
                outBytes = Marshal.AllocHGlobal(frac1Bytes.Length),
                bytesLen = (UIntPtr)frac1Bytes.Length
            };
            Fraction frac2 = new()
            {
                numerator = 9,
                denominator = 17,
                str = "World!",
                printFunc = CsPrintFunc,
                inBytes = Marshal.AllocHGlobal(frac2Bytes.Length),
                outBytes = Marshal.AllocHGlobal(frac2Bytes.Length),
                bytesLen = (UIntPtr)frac2Bytes.Length
            };
            Marshal.Copy(frac1Bytes, 0, frac1.inBytes, frac1Bytes.Length);
            Marshal.Copy(frac2Bytes, 0, frac2.inBytes, frac2Bytes.Length);
            int retval = fraction_multiply(ref frac1, ref frac2);
            Console.WriteLine("10/13 * 9/17 = " + frac1.numerator + "/" + frac1.denominator);
            byte[] frac1Result = new byte[frac1Bytes.Length];
            Marshal.Copy(frac1.outBytes, frac1Result, 0, frac1Bytes.Length);
            byte[] frac2Result = new byte[frac2Bytes.Length];
            Marshal.Copy(frac2.outBytes, frac2Result, 0, frac2Bytes.Length);
            Console.WriteLine("b'somedata' XOR 0x5c = 0x" + Convert.ToHexStringLower(frac1Result));
            Console.WriteLine("b'somemoredata' XOR 0x5c = 0x" + Convert.ToHexStringLower(frac2Result));
            Console.WriteLine("Error code = " + retval);
            Marshal.FreeHGlobal(frac1.inBytes);
            Marshal.FreeHGlobal(frac2.inBytes);
            Marshal.FreeHGlobal(frac1.outBytes);
            Marshal.FreeHGlobal(frac2.outBytes);
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Fraction
    {
        public int numerator, denominator;
        public string str;
        public FractionTester.PrintFuncType printFunc;
        public IntPtr inBytes, outBytes;
        public UIntPtr bytesLen;
    }
}
