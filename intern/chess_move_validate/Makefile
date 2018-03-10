COMPILER_FLAGS = "-Wall"
COMPILER_FLAGS += "-Wpedantic"
COMPILER_FLAGS += "-Wextra"
COMPILER_FLAGS += "-O3"
COMPILER_FLAGS += "-Wshadow"

GCC = gcc

chess_test.out: *.c
	${GCC} ${COMPILER_FLAGS} -o $@ $^

lib: *.c
	${GCC} ${COMPILER_FLAGS} -c $^
