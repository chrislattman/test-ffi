SHELL=/bin/bash

OS=$(shell echo `uname`)
PWD=$(shell pwd)

ifeq ($(OS),Darwin)
CC=gcc-13
JAVAC=/usr/local/Cellar/openjdk/21.0.1/bin/javac
JAVA=/usr/local/Cellar/openjdk/21.0.1/bin/java
INCLUDE=/usr/local/Cellar/openjdk/21.0.1/include # this is for jni.h
PYTHON3=/usr/local/bin/python3
LIBEXT=.dylib
else
CC=gcc
JAVAC=/usr/bin/javac
JAVA=/usr/bin/java
INCLUDE=/usr/lib/jvm/java-17-openjdk-amd64/include # this is for jni.h
PYTHON3=/usr/bin/python3
LIBEXT=.so
endif

CFLAGS=-Wall -Wextra -pedantic -std=c99 -shared -fpic

java: libfraction
	$(CC) $(CFLAGS) -I$(INCLUDE) -o java_ffi/libjava_ffi$(LIBEXT) java_ffi/java_ffi_FractionTester.c
	$(JAVA) -Djava.library.path=$(PWD)/java_ffi java_ffi/FractionTester.java

python: libfraction
	$(PYTHON3) python_ffi/fraction_tester.py

go: libfraction
	$(CC) $(CFLAGS) -o go_ffi/libgo_ffi$(LIBEXT) go_ffi/go_ffi.c
	# To build the executable separately:
	# cd go_ffi; go build
	# go_ffi/go_ffi
	#
	# To build and run in one command:
	go run go_ffi/fraction_tester.go go_ffi/cfuncs.go

nodejs: libfraction
	npm --prefix nodejs_ffi/ install
	npx tsc --strict nodejs_ffi/fraction_tester.ts
	node nodejs_ffi/

libfraction:
	$(CC) $(CFLAGS) -o libfraction$(LIBEXT) libfraction.c

clean:
	rm -rf *$(LIBEXT) **/*$(LIBEXT) nodejs_ffi/*.js nodejs_ffi/build go_ffi/go_ffi
