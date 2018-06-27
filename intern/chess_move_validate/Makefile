COMPILER_FLAGS = "-Wall"
COMPILER_FLAGS += "-Wpedantic"
COMPILER_FLAGS += "-Wextra"
COMPILER_FLAGS += "-O3"
COMPILER_FLAGS += "-Wshadow"
SHARED_LIB_FLAGS="-shared"

GCC = gcc
.PHONY: objs

libchess-move-validate.so: objs
	${GCC} ${SHARED_LIB_FLAGS} -Wl,-soname,$@ -o $@ chess.o chess_coins.o chess_helpers.o

objs: chess.c chess_coins.c chess_helpers.c
	${GCC} ${COMPILER_FLAGS} -fPIC -c $^

chess_test.out: *.c
	${GCC} ${COMPILER_FLAGS} -o $@ $^
