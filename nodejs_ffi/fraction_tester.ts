var nodejs_ffi = require("bindings")("nodejs_ffi.node");

class Fraction {
    numerator: number;
    denominator: number;
    str: string;
    printFunc: Function;

    constructor(numerator: number, denominator: number, str: string, printFunc: Function) {
        this.numerator = numerator;
        this.denominator = denominator;
        this.str = str;
        this.printFunc = printFunc;
    }
}

function nodejsPrint(argString: string): void {
    console.log(argString);
}

let frac1 = new Fraction(10, 13, "Hello", nodejsPrint);
const frac2 = new Fraction(9, 17, "World!", nodejsPrint);
const retval = nodejs_ffi.nodejs_fraction_multiply(frac1, frac2);
console.log(`10/13 * 9/17 = ${frac1.numerator}/${frac1.denominator}`);
console.log(`Error code = ${retval}`);
