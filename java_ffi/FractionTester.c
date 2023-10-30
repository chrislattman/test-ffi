#include <jni.h>
#include <dlfcn.h>

typedef struct fraction {
    int numerator, denominator;
} Fraction;

JNIEXPORT jint JNICALL Java_FractionTester_fractionMultiply(
        JNIEnv *env, jobject thisObject, jobject frac1, jobject frac2) {
    // Local variables
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);
    jclass frac1Class, frac2Class;
    jfieldID frac1NumeratorID, frac2NumeratorID, frac1DenominatorID, frac2DenominatorID;
    int frac1Numerator, frac2Numerator, frac1Denominator, frac2Denominator;
    Fraction f1, f2;
    int retval;

    // Integer signature from https://docs.oracle.com/en/java/javase/21/docs/specs/jni/types.html#type-signatures
    const char* sig = "I";

    // Retrieve symbol
    handle = dlopen("../libfraction.so", RTLD_NOW);
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    // Obtains the class of the frac objects
    frac1Class = (*env)->GetObjectClass(env, frac1);
    frac2Class = (*env)->GetObjectClass(env, frac2);

    // Obtains the numerator and denominator IDs from the frac objects
    frac1NumeratorID = (*env)->GetFieldID(env, frac1Class, "numerator", sig);
    frac1DenominatorID = (*env)->GetFieldID(env, frac1Class, "denominator", sig);
    frac2NumeratorID = (*env)->GetFieldID(env, frac2Class, "numerator", sig);
    frac2DenominatorID = (*env)->GetFieldID(env, frac2Class, "denominator", sig);

    // Obtains the actual integer values from the IDs above
    // These are jints which are typedef'd ints
    frac1Numerator = (*env)->GetIntField(env, frac1, frac1NumeratorID);
    frac1Denominator = (*env)->GetIntField(env, frac1, frac1DenominatorID);
    frac1Numerator = (*env)->GetIntField(env, frac1, frac2NumeratorID);
    frac2Denominator = (*env)->GetIntField(env, frac1, frac2DenominatorID);

    // Call the function
    f1 = (Fraction) { .numerator = frac1Numerator, .denominator = frac1Denominator};
    f2 = (Fraction) { .numerator = frac2Numerator, .denominator = frac2Denominator};
    retval = fraction_multiply(&f1, &f2);

    // Set frac1's fields to values from f1
    (*env)->SetIntField(env, frac1, frac1NumeratorID, f1.numerator);
    (*env)->SetIntField(env, frac1, frac1DenominatorID, f1.denominator);

    // Close file and return error code
    dlclose(handle);
    return retval;
}
