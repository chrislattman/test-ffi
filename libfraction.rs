type PrintFuncT = fn(String);

#[repr(C)]
pub struct Fraction {
    numerator: i32,
    denominator: i32,
    frac_str: String,
    print_func: PrintFuncT,
}

#[no_mangle]
pub extern "C" fn fraction_multiply(frac1: *mut Fraction, frac2: *mut Fraction) -> i32 {
    if !frac1.is_null() && !frac2.is_null() {
        unsafe {
            let numerator = (*frac1).numerator * (*frac2).numerator;
            let denominator = (*frac1).denominator * (*frac2).denominator;
            (*frac1).numerator = numerator;
            (*frac1).denominator = denominator;
            // ((*frac1).print_func)((*frac1).frac_str);
            // ((*frac2).print_func)((*frac2).frac_str);
            println!("Finished with calculation!");
            return 0;
        }
    }
    return -1;
}
