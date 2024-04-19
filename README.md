# Instructions

Foreign function interfaces (FFIs) are ways for programs written in a high-level language like Java, Python, JavaScript/TypeScript (Node.js), Ruby, PHP, and many others, to call functions written in low-level languages like C or C++.

This is often needed for performance requirements, where a Java/Python/Node.js/Ruby/PHP project needs to run some specific functions super efficiently, closer to the CPU level than the high-level languages offer. Other times, a well-maintained library is only written in C/C++ and it's not worth the effort to rewrite that library in a high-level language.

This project demonstrates FFIs for Java, Python, Node.js, Go, and Rust. In this contrived example, they each call a multiply function from a fraction library (libfraction.c). That function prints out a provided string using a print function passed down from the high-level language, then returns the result of the fraction operation.

Build and run the examples using the provided Makefile.

Note: C and C++ are not the only languages that can be called by high-level languages. Go and Rust support exporting code in library format. This repository contains support for calling functions from libfraction.go and libfraction.rs.
