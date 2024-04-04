use std::ffi::{c_char, c_int, c_void, CStr, CString};

#[link(name = "fraction")]
extern "C" {
    fn fraction_multiply(frac1: *mut c_void, frac2: *mut c_void) -> c_int;
}

#[repr(C)]
struct Fraction {
    numerator: c_int,
    denominator: c_int,
    string: *const c_char,
    print_func: fn(*const c_char),
}

fn rust_print(arg_string: *const c_char) {
    let result = unsafe { CStr::from_ptr(arg_string) };
    println!("{}", String::from_utf8_lossy(result.to_bytes()).to_string());
}

fn main() {
    let hello = CString::new("Hello").unwrap();
    let world = CString::new("World!").unwrap();
    let ptr1 = hello.as_ptr();
    let ptr2 = world.as_ptr();
    let mut frac1 = Fraction {
        numerator: 10,
        denominator: 13,
        string: ptr1,
        print_func: rust_print,
    };
    let mut frac2 = Fraction {
        numerator: 9,
        denominator: 17,
        string: ptr2,
        print_func: rust_print,
    };
    let frac1_ptr: *mut Fraction = &mut frac1;
    let frac1_void_p = frac1_ptr as *mut c_void;
    let frac2_ptr: *mut Fraction = &mut frac2;
    let frac2_void_p = frac2_ptr as *mut c_void;
    let retval = unsafe { fraction_multiply(frac1_void_p, frac2_void_p) };
    println!("10/13 * 9/17 = {}/{}", frac1.numerator, frac1.denominator);
    println!("Error code = {}", retval);
}
