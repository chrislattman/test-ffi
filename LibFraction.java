import java.util.List;

import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.CContext;
import org.graalvm.nativeimage.c.function.CEntryPoint;
import org.graalvm.nativeimage.c.function.CFunctionPointer;
import org.graalvm.nativeimage.c.function.InvokeCFunctionPointer;
import org.graalvm.nativeimage.c.struct.CField;
import org.graalvm.nativeimage.c.struct.CStruct;
import org.graalvm.nativeimage.c.type.CCharPointer;
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
            System.out.println("Finished with calculation!");
            return 0;
        }
        return -1;
    }
}
