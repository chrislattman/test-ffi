use std::{
    ffi::{c_char, c_int, c_uchar, c_ulong},
    slice,
};

#[repr(C)]
pub struct Fraction {
    numerator: c_int,
    denominator: c_int,
    frac_str: *const c_char,
    print_func: Option<fn(*const c_char)>,
    bytes: *const c_uchar,
    bytes_len: c_ulong, // This should be c_size_t but it's currently an unstable feature
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
            if let Some(frac1print) = (*frac1).print_func
                && !(*frac1).frac_str.is_null()
            {
                frac1print((*frac1).frac_str);
            }
            if let Some(frac2print) = (*frac2).print_func
                && !(*frac2).frac_str.is_null()
            {
                frac2print((*frac2).frac_str);
            }
            let frac1_bytes = slice::from_raw_parts((*frac1).bytes, (*frac1).bytes_len as usize);
            let frac2_bytes = slice::from_raw_parts((*frac2).bytes, (*frac2).bytes_len as usize);
            let mut counter = 0usize;
            for &elem in frac1_bytes {
                counter += elem as usize;
            }
            for &elem in frac2_bytes {
                counter += elem as usize;
            }
            println!(
                "The average of the bytes in frac1 and frac2 = {}",
                counter / (frac1_bytes.len() + frac2_bytes.len())
            );
            println!("Finished with calculation!");
            return 0;
        }
    }
    -1
}
