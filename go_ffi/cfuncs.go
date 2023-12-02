package main

// The multi-line comment below is a cgo preamble
/*
extern void go_print(const char *);

void print_cgo(const char *arg_str) {
    go_print(arg_str);
}
*/
import "C"
