// This is the "glue code" that loads the libfraction shared library
#include <node_api.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static napi_env g_env;
static napi_value g_global, g_frac1_print_func_napi, g_frac2_print_func_napi;

// Mimics the structure in libfraction.c
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
} Fraction;

void frac1_print_func(const char *arg_str) {
    napi_value str_napi, result;

    napi_create_string_utf8(g_env, arg_str, strlen(arg_str), &str_napi);
    napi_call_function(g_env, g_global, g_frac1_print_func_napi, 1, &str_napi, &result);
}

void frac2_print_func(const char *arg_str) {
    napi_value str_napi, result;

    napi_create_string_utf8(g_env, arg_str, strlen(arg_str), &str_napi);
    napi_call_function(g_env, g_global, g_frac2_print_func_napi, 1, &str_napi, &result);
}

static napi_value nodejs_fraction_multiply(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2], global;

    char *libpath = NULL;
    size_t dirlen;
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);

    napi_value frac1, frac1_numerator_napi, frac1_denominator_napi, frac1_str_napi, frac1_print_func_napi;
    napi_value frac2, frac2_numerator_napi, frac2_denominator_napi, frac2_str_napi, frac2_print_func_napi;
    int32_t frac1_numerator, frac1_denominator;
    int32_t frac2_numerator, frac2_denominator;

    size_t str_length;
    char *frac1_str = NULL, *frac2_str = NULL;
    napi_value result;
    Fraction f1, f2;
    int retval;

    napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    g_env = env;

    napi_get_global(env, &global);
    g_global = global;

    frac1 = args[0];
    frac2 = args[1];

    napi_create_int32(env, -1, &result);

    // Retrieve function symbol
    // uses malloc to allocate a buffer with exactly the right size
    libpath = getcwd(NULL, 0);
    if (libpath == NULL) {
        goto cleanup;
    }
    dirlen = strlen(libpath);
#ifdef __APPLE__
    strncat(libpath + dirlen, "/libfraction.dylib", sizeof(libpath) - dirlen - 1);
#else
    strncat(libpath + dirlen, "/libfraction.so", sizeof(libpath) - dirlen - 1);
#endif
    handle = dlopen(libpath, RTLD_NOW);
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    // Get the N-API values from frac1 and frac2
    napi_get_named_property(env, frac1, "numerator", &frac1_numerator_napi);
    napi_get_named_property(env, frac1, "denominator", &frac1_denominator_napi);
    napi_get_named_property(env, frac1, "str", &frac1_str_napi);
    napi_get_named_property(env, frac1, "print_func", &frac1_print_func_napi);
    napi_get_named_property(env, frac2, "numerator", &frac2_numerator_napi);
    napi_get_named_property(env, frac2, "denominator", &frac2_denominator_napi);
    napi_get_named_property(env, frac2, "str", &frac2_str_napi);
    napi_get_named_property(env, frac2, "print_func", &frac2_print_func_napi);

    // Get the actual values from the N-API values
    napi_get_value_int32(env, frac1_numerator_napi, &frac1_numerator);
    napi_get_value_int32(env, frac1_denominator_napi, &frac1_denominator);
    napi_get_value_int32(env, frac2_numerator_napi, &frac2_numerator);
    napi_get_value_int32(env, frac2_denominator_napi, &frac2_denominator);

    // Extracts the strings from the N-API objects
    napi_get_value_string_utf8(env, frac1_str_napi, NULL, 0, &str_length);
    frac1_str = calloc(str_length + 1, sizeof(char));
    if (frac1_str == NULL) {
        goto cleanup;
    }
    napi_get_value_string_utf8(env, frac1_str_napi, frac1_str, str_length + 1, &str_length);
    napi_get_value_string_utf8(env, frac2_str_napi, NULL, 0, &str_length);
    frac2_str = calloc(str_length + 1, sizeof(char));
    if (frac2_str == NULL) {
        goto cleanup;
    }
    napi_get_value_string_utf8(env, frac2_str_napi, frac2_str, str_length + 1, &str_length);
    g_frac1_print_func_napi = frac1_print_func_napi;
    g_frac2_print_func_napi = frac2_print_func_napi;

    // Call the function
    f1 = (Fraction) {
        .numerator = frac1_numerator,
        .denominator = frac1_denominator,
        .str = frac1_str,
        .print_func = frac1_print_func
    };
    f2 = (Fraction) {
        .numerator = frac2_numerator,
        .denominator = frac2_denominator,
        .str = frac2_str,
        .print_func = frac2_print_func
    };
    retval = fraction_multiply(&f1, &f2);
    napi_create_int32(env, retval, &result);

    // Set frac1's properties to values from f1
    napi_create_int32(env, f1.numerator, &frac1_numerator_napi);
    napi_create_int32(env, f1.denominator, &frac1_denominator_napi);
    napi_set_named_property(env, frac1, "numerator", frac1_numerator_napi);
    napi_set_named_property(env, frac1, "denominator", frac1_denominator_napi);

cleanup:
    // Free strings, close file and return error code
    if (frac1_str != NULL) {
        free(frac1_str);
    }
    if (frac2_str != NULL) {
        free(frac2_str);
    }
    dlclose(handle);
    if (libpath != NULL) {
        free(libpath);
    }
    return result;
}

#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("nodejs_fraction_multiply", nodejs_fraction_multiply);
    napi_define_properties(env, exports, 1, &addDescriptor);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
