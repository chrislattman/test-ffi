// This is the "glue code" that calls libfraction.so
#include <dlfcn.h>
#include "java_ffi_FractionTester.h" // this file was generated with: javac -h java_ffi java_ffi/*.java

static JNIEnv *g_env;
static jobject g_frac1, g_frac2;
static jmethodID g_frac1PrintFuncID, g_frac2PrintFuncID;

// Mimics the structure in libfraction.c
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
} Fraction;

void frac1_print_func(const char *arg_str) {
    jvalue frac1PrintArgs[1];

    frac1PrintArgs[0].l = (*g_env)->NewStringUTF(g_env, arg_str);
    (*g_env)->CallVoidMethodA(g_env, g_frac1, g_frac1PrintFuncID, frac1PrintArgs);
}

void frac2_print_func(const char *arg_str) {
    jvalue frac2PrintArgs[1];

    frac2PrintArgs[0].l = (*g_env)->NewStringUTF(g_env, arg_str);
    (*g_env)->CallVoidMethodA(g_env, g_frac2, g_frac2PrintFuncID, frac2PrintArgs);
}

JNIEXPORT jint JNICALL Java_java_1ffi_FractionTester_fractionMultiply(
        JNIEnv *env, jobject thisObject, jobject frac1, jobject frac2) {
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);

    jclass frac1Class, frac2Class;
    jfieldID frac1NumeratorID, frac2NumeratorID, frac1DenominatorID, frac2DenominatorID, frac1StrID, frac2StrID;
    jint frac1Numerator, frac2Numerator, frac1Denominator, frac2Denominator;
    jobject frac1StrObj, frac2StrObj;
    jboolean frac1StrIsCopy, frac2StrIsCopy; // not used

    const char *frac1Str, *frac2Str;
    Fraction f1, f2;
    int retval;

    // Integer signature from https://docs.oracle.com/en/java/javase/21/docs/specs/jni/types.html#type-signatures
    const char *int_sig = "I";

    // String signature from https://docs.oracle.com/en/java/javase/21/docs/specs/jni/types.html#type-signatures
    const char *str_sig = "Ljava/lang/String;";

    // Function pointer signature (V means void return type)
    const char *func_ptr_sig = "(Ljava/lang/String;)V";

    // Retrieve function symbol
    handle = dlopen("../libfraction.so", RTLD_NOW);
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    // Obtains the jclass of the Fraction jobjects
    frac1Class = (*env)->GetObjectClass(env, frac1);
    frac2Class = (*env)->GetObjectClass(env, frac2);

    // Obtains the field IDs from the Fraction jobjects
    frac1NumeratorID = (*env)->GetFieldID(env, frac1Class, "numerator", int_sig);
    frac1DenominatorID = (*env)->GetFieldID(env, frac1Class, "denominator", int_sig);
    frac2NumeratorID = (*env)->GetFieldID(env, frac2Class, "numerator", int_sig);
    frac2DenominatorID = (*env)->GetFieldID(env, frac2Class, "denominator", int_sig);
    frac1StrID = (*env)->GetFieldID(env, frac1Class, "str", str_sig);
    frac2StrID = (*env)->GetFieldID(env, frac2Class, "str", str_sig);

    // Obtains the actual integer values from the IDs above
    // These are jints which are typedef'd ints
    frac1Numerator = (*env)->GetIntField(env, frac1, frac1NumeratorID);
    frac1Denominator = (*env)->GetIntField(env, frac1, frac1DenominatorID);
    frac2Numerator = (*env)->GetIntField(env, frac2, frac2NumeratorID);
    frac2Denominator = (*env)->GetIntField(env, frac2, frac2DenominatorID);

    // Extracts the strings from the Fraction jobjects
    frac1StrObj = (*env)->GetObjectField(env, frac1, frac1StrID);
    frac1Str = (*env)->GetStringUTFChars(env, frac1StrObj, &frac1StrIsCopy);
    frac2StrObj = (*env)->GetObjectField(env, frac2, frac2StrID);
    frac2Str = (*env)->GetStringUTFChars(env, frac2StrObj, &frac2StrIsCopy);

    // Obtains the method IDs of the Java callback functions and sets
    // global variables used by callback functions
    g_frac1PrintFuncID = (*env)->GetMethodID(env, frac1Class, "printFunc", func_ptr_sig);
    g_frac2PrintFuncID = (*env)->GetMethodID(env, frac2Class, "printFunc", func_ptr_sig);
    g_env = env;
    g_frac1 = frac1;
    g_frac2 = frac2;

    // Call the function
    f1 = (Fraction) {
        .numerator = frac1Numerator,
        .denominator = frac1Denominator,
        .str = frac1Str,
        .print_func = frac1_print_func
    };
    f2 = (Fraction) {
        .numerator = frac2Numerator,
        .denominator = frac2Denominator,
        .str = frac2Str,
        .print_func = frac2_print_func
    };
    retval = fraction_multiply(&f1, &f2);

    // Set frac1's fields to values from f1
    (*env)->SetIntField(env, frac1, frac1NumeratorID, f1.numerator);
    (*env)->SetIntField(env, frac1, frac1DenominatorID, f1.denominator);

    // Close file and return error code
    dlclose(handle);
    return retval;
}
