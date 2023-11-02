// This is the "glue code" that calls libfraction.so
#include <node_api.h>
#include <dlfcn.h>

// Mimics the structure in libfraction.c
typedef struct fraction {
    int numerator, denominator;
    const char *str;
    void (*print_func)(const char *);
} Fraction;

static napi_value nodejs_fraction_multiply(napi_env env, napi_callback_info info) {
    size_t argc;
    napi_value args[2];
    void *handle;
    int (*fraction_multiply)(Fraction *, Fraction *);
    Fraction f1, f2;
    int retval;
    napi_value result;

    napi_get_cb_info(env, info, &argc, args, NULL, NULL);

    double value0;
    napi_get_value_double(env, args[0], &value0);

    double value1;
    napi_get_value_double(env, args[1], &value1);

    napi_value sum;
    napi_create_double(env, value0 + value1, &sum);

    handle = dlopen("../libfraction.so", RTLD_NOW);
    *(void **)(&fraction_multiply) = dlsym(handle, "fraction_multiply");

    // retval = fraction_multiply(f1, f2);
    // store f1 results in argument

    dlclose(handle);
    // napi_create_int32(env, retval, &result);
    // return result;
    return sum;
}

#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("nodejs_fraction_multiply", nodejs_fraction_multiply);
    napi_define_properties(env, exports, 1, &addDescriptor);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
