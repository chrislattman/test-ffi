SHELL=/bin/bash

OS=$(shell echo `uname`)
PWD=$(shell pwd)

ifeq ($(OS),Darwin)
CC=gcc-13
JAVAC=/usr/local/Cellar/openjdk/21.0.1/bin/javac
INCLUDE=/usr/local/Cellar/openjdk/21.0.1/include # this is for jni.h
LIBNAME=libnative.dylib
JAVA=/usr/local/Cellar/openjdk/21.0.1/bin/java
PYTHON3=/usr/local/bin/python3
else
CC=gcc
JAVAC=/usr/bin/javac
INCLUDE=/usr/lib/jvm/java-17-openjdk-amd64/include # this is for jni.h
LIBNAME=libnative.so
JAVA=/usr/bin/java
PYTHON3=/usr/bin/python3
endif

java: libfraction
	$(JAVAC) java_ffi/FractionTester.java
	$(CC) -shared -fpic -I $(INCLUDE) -o java_ffi/$(LIBNAME) java_ffi/java_ffi_FractionTester.c
	$(JAVA) -Djava.library.path=$(PWD)/java_ffi java_ffi.FractionTester

libfraction:
	$(CC) -shared -fpic -o libfraction.so libfraction.c

clean:
	rm -rf *.so java_ffi/*.class java_ffi/*.so java_ffi/*.dylib
