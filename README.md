# Instructions

Foreign function interfaces (FFIs) are ways for programs written in a high-level language like Java, Python, JavaScript/TypeScript (Node.js), Ruby, PHP, and many others, to call functions written in low-level languages like C or C++.

This is often needed for performance requirements, where a Java/Python/Node.js/Ruby/PHP project needs to run some specific functions super efficiently, closer to the CPU level than the high-level languages offer.

Other times, a well-maintained library is only written in C/C++ and it's not worth the effort to rewrite that library in a high-level language.

This project demonstrates FFIs for Java, Python, Node.js, and Go. In this contrived example, they each call a multiply function from a fraction library (libfraction.c). That function prints out a provided string using a print function passed down from the high-level language, then returns the result of the fraction operation.

Build and run the examples using the provided Makefile.

Note: currently, the Node.js example doesn't work.
