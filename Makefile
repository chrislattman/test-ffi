SHELL=/bin/bash

OS=$(shell echo `uname -a`)
PWD=$(shell pwd)

ifneq ($(findstring x86_64,$(OS)),)
INCLUDE=/usr/local/Cellar/openjdk/21.0.1/include # this is for jni.h
ARCH=amd64
else
INCLUDE=/opt/homebrew/opt/openjdk/include # this is for jni.h
ARCH=arm64
endif

ifneq ($(findstring Darwin,$(OS)),)
CC=gcc-13
LIBEXT=.dylib
else
CC=gcc
LIBEXT=.so
INCLUDE=/usr/lib/jvm/java-17-openjdk-$(ARCH)/include # this is for jni.h
INCLUDE2=-I/usr/lib/jvm/java-17-openjdk-$(ARCH)/include/linux # this is for jni_md.h
endif

CFLAGS=-Wall -Wextra -pedantic -std=c99 -shared -fpic

java: libfraction
	$(CC) $(CFLAGS) -I$(INCLUDE) $(INCLUDE2) -o java_ffi/libjava_ffi$(LIBEXT) java_ffi/java_ffi_FractionTester.c
	java -Djava.library.path=$(PWD)/java_ffi java_ffi/FractionTester.java

python: libfraction
	python3 python_ffi/fraction_tester.py

go: libfraction
	$(CC) $(CFLAGS) -o go_ffi/libgo_ffi$(LIBEXT) go_ffi/go_ffi.c
	# To build the executable separately:
	# cd go_ffi; go build
	# go_ffi/go_ffi
	#
	# To build and run in one command:
	go run go_ffi/fraction_tester.go go_ffi/cfuncs.go

# Doesn't work yet
nodejs: libfraction
	npm --prefix nodejs_ffi/ install
	npx tsc --strict nodejs_ffi/fraction_tester.ts
	node nodejs_ffi/

libfraction:
	$(CC) $(CFLAGS) -o libfraction$(LIBEXT) libfraction.c

clean:
	rm -rf *$(LIBEXT) **/*$(LIBEXT) nodejs_ffi/*.js nodejs_ffi/build go_ffi/go_ffi
