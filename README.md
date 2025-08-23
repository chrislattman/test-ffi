# Foreign Function Interfaces (FFIs)

Foreign function interfaces (FFIs) are ways for programs written in a high-level language like Java, Python, JavaScript/TypeScript (Node.js), Ruby, PHP, and many others, to call functions written in low-level languages like C or C++.

This is often needed for performance requirements, where a Java/Python/Node.js/Ruby/PHP project needs to run some specific functions super efficiently, closer to the CPU level than the high-level languages offer. Other times, a well-maintained library is only written in C/C++ and it's not worth the effort to rewrite that library in a high-level language.

This project demonstrates FFIs for Java, Python, Node.js, Go, Rust, and WebAssembly (wasm). In this contrived example, they each call a multiply function from a fraction library (libfraction.c). That function prints out a provided string using a print function passed down from the high-level language, then returns the result of the fraction operation.

- This project dynamically loads the shared library, but other use cases involve compiling the existing C/C++ project and the "glue code" into one large shared library
- This is more practical when dealing with projects with lots of structs and function definitions, which are much easier to use when exposed by header files

Build and run the examples using the provided Makefile.

Note: C and C++ are not the only languages that can be called by high-level languages. Go and Rust support exporting code in library format. This repository contains support for calling functions from libfraction.go and libfraction.rs.

Surprisingly enough, it's possible to compile Java and C# code into native libraries. GraalVM accomplishes this for Java, and an example is located in this repository (the Makefile assumes GraalVM is installed and set as the default Java compiler). C# has built-in support for native library compilation. Python 3 has unofficial native library support via PyInstaller and there is currently no official support for Node.js native binaries.

For the WebAssembly example, the print function isn't passed down, but it's made available as a module function. To view the results, open http://localhost:8000/webpage.html in a web browser, right click and inspect the page (opens DevTools), and open the console. Then refresh the page.
