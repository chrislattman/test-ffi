SHELL=/bin/bash

OS=$(shell echo `uname -a`)
PWD=$(shell pwd)

ifneq ($(findstring x86_64,$(OS)),)
INCLUDE=/usr/local/Cellar/openjdk/23.0.1/include # this is for jni.h
ARCH=amd64
else
INCLUDE=/opt/homebrew/opt/openjdk/include # this is for jni.h
ARCH=arm64
endif

ifneq ($(findstring Darwin,$(OS)),)
CC=gcc-14
LIBEXT=.dylib
else
CC=gcc
LIBEXT=.so
INCLUDE=/usr/lib/jvm/java-21-openjdk-$(ARCH)/include # this is for jni.h
INCLUDE2=-I/usr/lib/jvm/java-21-openjdk-$(ARCH)/include/linux # this is for jni_md.h
endif

CFLAGS=-Wall -Wextra -pedantic -std=c99 -shared -fpic

ifeq ($(LIB_GO),1)
LIB=libfraction_go
else ifeq ($(LIB_RUST),1)
LIB=libfraction_rust
else
LIB=libfraction
endif

java: $(LIB)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE2) -o java_ffi/libjava_ffi$(LIBEXT) java_ffi/java_ffi_FractionTester.c
	javac java_ffi/FractionTester.java
	java -Djava.library.path=$(PWD)/java_ffi java_ffi/FractionTester
	# java -Djava.library.path=$(PWD)/java_ffi java_ffi/FractionTester.java

python: $(LIB)
	python3 python_ffi/fraction_tester.py

nodejs: $(LIB)
	npx node-gyp rebuild --loglevel silent
	npm start

go: $(LIB)
	$(CC) $(CFLAGS) -o go_ffi/libgo_ffi$(LIBEXT) go_ffi/go_ffi.c
	cd go_ffi; go build
	go_ffi/go_ffi
	# go run go_ffi/fraction_tester.go go_ffi/cfuncs.go

rust: $(LIB)
ifneq ($(findstring Darwin,$(OS)),)
	RUSTFLAGS="-L." cargo run -q
else
	RUSTFLAGS="-L. -C link-args=-Wl,-rpath=$(PWD)" cargo run -q
endif

webassembly: $(LIB)
	# Compiling libfraction.c with wasm_ffi.c since web browsers cannot run ELF binaries
	cd webassembly_ffi; emcc -sEXPORTED_RUNTIME_METHODS=["cwrap"] wasm_ffi.c ../libfraction.c; python3 -m http.server

libfraction:
	$(CC) $(CFLAGS) -o libfraction$(LIBEXT) libfraction.c

libfraction_go:
	go build -buildmode=c-shared -o libfraction$(LIBEXT) libfraction.go
	rm libfraction.h

libfraction_rust:
	rustc --crate-type=cdylib -o libfraction$(LIBEXT) libfraction.rs

clean:
	rm -rf *$(LIBEXT) **/*$(LIBEXT) go_ffi/go_ffi build java_ffi/*.class target webassembly_ffi/a.out*

help:
	@echo "To use the Go fraction library add LIB_GO=1, e.g. make python LIB_GO=1"
