use std::ffi::{c_char, c_int};

#[repr(C)]
pub struct Fraction {
    numerator: c_int,
    denominator: c_int,
    frac_str: *const c_char,
    print_func: fn(*const c_char),
}

/// # Safety
///
/// `frac1` and `frac2` should not be null
#[unsafe(no_mangle)]
pub unsafe extern "C" fn fraction_multiply(frac1: *mut Fraction, frac2: *mut Fraction) -> i32 {
    if !frac1.is_null() && !frac2.is_null() {
        unsafe {
            let numerator = (*frac1).numerator * (*frac2).numerator;
            let denominator = (*frac1).denominator * (*frac2).denominator;
            (*frac1).numerator = numerator;
            (*frac1).denominator = denominator;
            ((*frac1).print_func)((*frac1).frac_str);
            ((*frac2).print_func)((*frac2).frac_str);
            println!("Finished with calculation!");
            return 0;
        }
    }
    -1
}
