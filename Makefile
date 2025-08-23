SHELL=/bin/bash

OS=$(shell echo `uname -a`)
PWD=$(shell pwd)

ifneq ($(findstring x86_64,$(OS)),)
INCLUDE=/usr/local/Cellar/openjdk/24.0.2/include # this is for jni.h
ARCH=amd64
else
INCLUDE=/opt/homebrew/opt/openjdk/include # this is for jni.h
ARCH=arm64
endif

ifneq ($(findstring Darwin,$(OS)),)
CC=gcc-15
CXX=g++-15
LIBEXT=.dylib
else
CC=gcc
CXX=g++
LIBEXT=.so
INCLUDE=/usr/lib/jvm/java-21-openjdk-$(ARCH)/include # this is for jni.h
INCLUDE2=-I/usr/lib/jvm/java-21-openjdk-$(ARCH)/include/linux # this is for jni_md.h
endif

CLIBFLAGS=-Wall -Wextra -pedantic -std=c99 -shared -fpic

ifeq ($(LIB_CPP),1)
LIB=libfraction_cpp
else ifeq ($(LIB_GO),1)
LIB=libfraction_go
else ifeq ($(LIB_RUST),1)
LIB=libfraction_rust
else ifeq ($(LIB_JAVA),1)
LIB=libfraction_java
else
LIB=libfraction
endif

java: $(LIB)
	$(CC) $(CLIBFLAGS) -I$(INCLUDE) $(INCLUDE2) -o java_ffi/libjava_ffi$(LIBEXT) java_ffi/java_ffi_FractionTester.c
	javac java_ffi/FractionTester.java
	java --enable-native-access=ALL-UNNAMED -Djava.library.path=$(PWD)/java_ffi java_ffi/FractionTester

csharp: $(LIB)
	dotnet run --project csharp_ffi

python: $(LIB)
	python3 python_ffi/fraction_tester.py

nodejs: $(LIB)
	npx node-gyp rebuild --loglevel silent
	npm start

go: $(LIB)
	$(CC) $(CLIBFLAGS) -o go_ffi/libgo_ffi$(LIBEXT) go_ffi/go_ffi.c
	cd go_ffi; go build
	go_ffi/go_ffi
	# go run go_ffi/fraction_tester.go go_ffi/cfuncs.go

rust: $(LIB)
ifneq ($(findstring Darwin,$(OS)),)
	RUSTFLAGS="-L." cargo run -q
else
	RUSTFLAGS="-L. -C link-args=-Wl,-rpath=$(PWD)" cargo run -q
endif

webassembly:
	# Compiling libfraction.c with wasm_ffi.c since web browsers cannot run ELF binaries
	cd webassembly_ffi; emcc -sEXPORTED_RUNTIME_METHODS=["cwrap"] wasm_ffi.c ../libfraction.c; python3 -m http.server

libfraction:
	$(CC) $(CLIBFLAGS) -o libfraction$(LIBEXT) libfraction.c

libfraction_cpp:
	$(CXX) -Wall -Wextra -pedantic -std=c++14 -shared -fpic -o libfraction$(LIBEXT) libfraction.cpp

libfraction_go:
	go build -buildmode=c-shared -o libfraction$(LIBEXT) libfraction.go
	rm libfraction.h

libfraction_rust:
	# rustc --crate-type=cdylib -o libfraction$(LIBEXT) libfraction.rs
	cargo build -q --lib && mv target/debug/libfraction$(LIBEXT) ./libfraction$(LIBEXT)

libfraction_java:
# 	javac LibEnvMap.java
# 	native-image --shared -o libenvmap
# 	$(CC) -I. -Wl,-rpath . -o envmap envmap.c ./libenvmap$(LIBEXT)
#   Then run ./main <ENV_VAR>
	echo "public class Constants { public static final String PWD = \"$(PWD)\"; }" > Constants.java
	javac Constants.java LibFraction.java
	native-image --shared -o libgraalvmfraction --silent
	rm *_dynamic.h *.class Constants.java
	$(CC) $(CLIBFLAGS) -I. -Wl,-rpath . -o libfraction$(LIBEXT) libgraalvmfraction.c ./libgraalvmfraction$(LIBEXT)
	rm graal_isolate.h libgraalvmfraction.h

clean:
	rm -rf *$(LIBEXT) **/*$(LIBEXT) go_ffi/go_ffi build java_ffi/*.class target webassembly_ffi/a.out* csharp_ffi/bin csharp_ffi/obj

help:
	@echo "To use the Go fraction library add LIB_GO=1, e.g. make python LIB_GO=1"
