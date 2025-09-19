use std::ffi::{CStr, CString, c_char, c_int, c_uchar, c_ulong, c_void};

#[link(name = "fraction")]
unsafe extern "C" {
    fn fraction_multiply(frac1: *mut c_void, frac2: *mut c_void) -> c_int;
}

#[repr(C)]
struct Fraction {
    numerator: c_int,
    denominator: c_int,
    string: *const c_char,
    print_func: Option<fn(*const c_char)>,
    in_bytes: *const c_uchar,
    out_bytes: *mut c_uchar,
    bytes_len: c_ulong,
}

fn rust_print(arg_string: *const c_char) {
    let result = unsafe { CStr::from_ptr(arg_string) };
    println!("{}", String::from_utf8_lossy(result.to_bytes()));
}

fn main() {
    let hello = CString::new("Hello").unwrap();
    let world = CString::new("World!").unwrap();
    let ptr1 = hello.as_ptr();
    let ptr2 = world.as_ptr();
    let frac1_in_bytes = b"somedata";
    let frac2_in_bytes = b"somemoredata";
    let mut frac1_out_bytes = vec![0u8; frac1_in_bytes.len()];
    let mut frac2_out_bytes = vec![0u8; frac2_in_bytes.len()];
    let mut frac1 = Fraction {
        numerator: 10,
        denominator: 13,
        string: ptr1,
        print_func: Some(rust_print),
        in_bytes: frac1_in_bytes.as_ptr(),
        out_bytes: frac1_out_bytes.as_mut_ptr(),
        bytes_len: frac1_in_bytes.len() as u64,
    };
    let mut frac2 = Fraction {
        numerator: 9,
        denominator: 17,
        string: ptr2,
        print_func: Some(rust_print),
        in_bytes: frac2_in_bytes.as_ptr(),
        out_bytes: frac2_out_bytes.as_mut_ptr(),
        bytes_len: frac2_in_bytes.len() as u64,
    };
    let frac1_ptr: *mut Fraction = &mut frac1;
    let frac1_void_p = frac1_ptr as *mut c_void;
    let frac2_ptr: *mut Fraction = &mut frac2;
    let frac2_void_p = frac2_ptr as *mut c_void;
    let retval = unsafe { fraction_multiply(frac1_void_p, frac2_void_p) };
    println!("10/13 * 9/17 = {}/{}", frac1.numerator, frac1.denominator);
    println!(
        "b'somedata' XOR 0x5c = 0x{}",
        frac1_out_bytes
            .iter()
            .map(|b| format!("{:02x}", b))
            .collect::<String>()
    );
    println!(
        "b'somemoredata' XOR 0x5c = 0x{}",
        frac2_out_bytes
            .iter()
            .map(|b| format!("{:02x}", b))
            .collect::<String>()
    );
    println!("Error code = {retval}");
}
