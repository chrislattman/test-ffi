SHELL=/bin/bash

OS=$(shell echo `uname`)
PWD=$(shell pwd)

ifeq ($(OS),Darwin)
CC=gcc-13
JAVAC=/usr/local/Cellar/openjdk/21.0.1/bin/javac
JAVA=/usr/local/Cellar/openjdk/21.0.1/bin/java
INCLUDE=/usr/local/Cellar/openjdk/21.0.1/include # this is for jni.h
LIBNATIVE=libnative.dylib
LIBGOFFI=libgoffi.dylib
PYTHON3=/usr/local/bin/python3
LIBFRACTION=libfraction.dylib
else
CC=gcc
JAVAC=/usr/bin/javac
JAVA=/usr/bin/java
INCLUDE=/usr/lib/jvm/java-17-openjdk-amd64/include # this is for jni.h
LIBNATIVE=libnative.so
LIBGOFFI=libgoffi.so
PYTHON3=/usr/bin/python3
LIBFRACTION=libfraction.so
endif

java: libfraction
	$(JAVAC) java_ffi/*.java
	$(CC) -shared -fpic -I$(INCLUDE) -o java_ffi/$(LIBNATIVE) java_ffi/java_ffi_FractionTester.c
	$(JAVA) -Djava.library.path=$(PWD)/java_ffi java_ffi.FractionTester

python: libfraction
	$(PYTHON3) python_ffi/fraction_tester.py

go: libfraction
	$(CC) -shared -fpic -o go_ffi/ffi_glue/$(LIBGOFFI) go_ffi/ffi_glue/go_ffi.c
	# go run go_ffi/fraction_tester.go

nodejs: libfraction
	npm --prefix nodejs_ffi/ install
	npx tsc --strict nodejs_ffi/fraction_tester.ts
	node nodejs_ffi/

libfraction:
	$(CC) -shared -fpic -o $(LIBFRACTION) libfraction.c

clean:
	rm -rf *.so *.dylib java_ffi/*.class java_ffi/*.so java_ffi/*.dylib go_ffi/ffi_glue/*.so go_ffi/ffi_glue/*.dylib nodejs_ffi/*.js nodejs_ffi/build
