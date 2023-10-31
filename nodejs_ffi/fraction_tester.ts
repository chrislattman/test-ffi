var nodejs_ffi = require("bindings")("nodejs_ffi.node")

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

function print(arg_string: string) {
    console.log(arg_string);
}

const frac1 = new Fraction(10, 13, "Hello", print);
const frac2 = new Fraction(9, 17, "World!", print);

console.log('This should be eight:', nodejs_ffi.nodejs_fraction_multiply(3, 5))
