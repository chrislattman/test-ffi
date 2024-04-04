struct Fraction {
    numerator: i32,
    denominator: i32,
    string: String,
    print_func: fn(String),
}

fn rust_print(arg_string: String) {
    println!("{}", arg_string);
}

fn main() {
    let mut frac1 = Fraction {
        numerator: 10,
        denominator: 13,
        string: "Hello".to_string(),
        print_func: rust_print,
    };
    let mut frac2 = Fraction {
        numerator: 9,
        denominator: 17,
        string: "World!".to_string(),
        print_func: rust_print,
    };
    println!("10/13 * 9/17 = {}/{}", frac1.numerator, frac1.denominator);
    println!("Error code = {}", 0);
}
