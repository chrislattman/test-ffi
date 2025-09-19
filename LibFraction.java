import java.nio.ByteBuffer;
import java.util.List;

import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.CContext;
import org.graalvm.nativeimage.c.function.CEntryPoint;
import org.graalvm.nativeimage.c.function.CFunctionPointer;
import org.graalvm.nativeimage.c.function.InvokeCFunctionPointer;
import org.graalvm.nativeimage.c.struct.CField;
import org.graalvm.nativeimage.c.struct.CStruct;
import org.graalvm.nativeimage.c.type.CCharPointer;
import org.graalvm.nativeimage.c.type.CTypeConversion;
import org.graalvm.word.PointerBase;

@CContext(CHeaderDirectives.class)
@CStruct("Fraction")
interface Fraction extends PointerBase {
    @CField("numerator")
    int getNumerator();

    @CField("numerator")
    void setNumerator(int n);

    @CField("denominator")
    int getDenominator();

    @CField("denominator")
    void setDenominator(int d);

    @CField("str")
    CCharPointer getStr();

    @CField("str")
    void setStr(CCharPointer str);

    @CField("print_func")
    PrintFunc getPrintFunc();

    @CField("print_func")
    void setPrintFunc(PrintFunc func);

    @CField("in_bytes")
    CCharPointer getInBytes();

    @CField("in_bytes")
    void setInBytes(CCharPointer bytes);

    @CField("out_bytes")
    CCharPointer getOutBytes();

    @CField("out_bytes")
    void setOutBytes(CCharPointer bytes);

    @CField("bytes_len")
    long getBytesLen();

    @CField("bytes_len")
    void setBytesLen(long len);
}

interface PrintFunc extends CFunctionPointer {
    @InvokeCFunctionPointer
    void invoke(CCharPointer s);
}

class CHeaderDirectives implements CContext.Directives {
    @Override
    public List<String> getHeaderFiles() {
        return List.of("<fraction.h>");
    }

    @Override
    public List<String> getOptions() {
        return List.of("-I" + Constants.PWD);
    }
}

public class LibFraction {
    @CEntryPoint(name = "graalvm_fraction_multiply")
    public static int graalvmFractionMultiply(IsolateThread thread, Fraction frac1, Fraction frac2) {
        if (frac1.isNonNull() && frac2.isNonNull()) {
            int numerator = frac1.getNumerator() * frac2.getNumerator();
            int denominator = frac1.getDenominator() * frac2.getDenominator();
            frac1.setNumerator(numerator);
            frac1.setDenominator(denominator);
            if (frac1.getPrintFunc().isNonNull() && frac1.getStr().isNonNull()) {
                frac1.getPrintFunc().invoke(frac1.getStr());
            }
            if (frac2.getPrintFunc().isNonNull() && frac2.getStr().isNonNull()) {
                frac2.getPrintFunc().invoke(frac2.getStr());
            }
            ByteBuffer frac1InByteBuffer = CTypeConversion.asByteBuffer(frac1.getInBytes(), (int) frac1.getBytesLen());
            ByteBuffer frac2InByteBuffer = CTypeConversion.asByteBuffer(frac2.getInBytes(), (int) frac2.getBytesLen());
            byte[] frac1InBytes = new byte[frac1InByteBuffer.limit()];
            byte[] frac2InBytes = new byte[frac2InByteBuffer.limit()];
            frac1InByteBuffer.get(frac1InBytes);
            frac2InByteBuffer.get(frac2InBytes);
            // Assuming out_bytes has been allocated by the caller of this function!
            ByteBuffer frac1OutByteBuffer = CTypeConversion.asByteBuffer(frac1.getOutBytes(), (int) frac1.getBytesLen());
            ByteBuffer frac2OutByteBuffer = CTypeConversion.asByteBuffer(frac2.getOutBytes(), (int) frac2.getBytesLen());
            int counter = 0;
            for (int i = 0; i < frac1InBytes.length; i++) {
                counter += frac1InBytes[i];
                frac1OutByteBuffer.put((byte)(frac1InBytes[i] ^ 0x5c));
            }
            for (int i = 0; i < frac2InBytes.length; i++) {
                counter += frac2InBytes[i];
                frac2OutByteBuffer.put((byte)(frac2InBytes[i] ^ 0x5c));
            }
            System.out.println("The average of the bytes in frac1 and frac2 = " + counter / (frac1InBytes.length + frac2InBytes.length));
            System.out.println("Finished with calculation!");
            return 0;
        }
        return -1;
    }
}
