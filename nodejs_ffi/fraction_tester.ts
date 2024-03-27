var nodejs_ffi = require("bindings")("nodejs_ffi.node");

class Fraction {
    numerator: number;
    denominator: number;
    str: string;
    print_func: Function;

    constructor(numerator: number, denominator: number, str: string, print_func: Function) {
        this.numerator = numerator;
        this.denominator = denominator;
        this.str = str;
        this.print_func = print_func;
    }
}

function nodejs_print(arg_string: string): void {
    console.log(arg_string);
}

const frac1 = new Fraction(10, 13, "Hello", nodejs_print);
const frac2 = new Fraction(9, 17, "World!", nodejs_print);
const retval = nodejs_ffi.nodejs_fraction_multiply(frac1, frac2);
console.log(`10/13 * 9/17 = ${frac1.numerator}/${frac1.denominator}`);
console.log(`Error code = ${retval}`);
