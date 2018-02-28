COMPILER_FLAGS = "-Wall"
COMPILER_FLAGS += "-Wpedantic"
LINKER_FLAGS = "-lm"

all: *.c
	gcc ${COMPILER_FLAGS} $^ -o $@ ${LINKER_FLAGS}

lib: math.c sqrt.c
	gcc ${COMPILER_FLAGS} -c $^ ${LINKER_FLAGS}
