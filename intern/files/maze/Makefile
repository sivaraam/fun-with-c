COMPILER_FLAGS = "-Wall"
COMPILER_FLAGS += "-Wpedantic"
COMPILER_FLAGS += "-Wextra"

BMP_LIBRARY = bmp/bmp_helpers.c

maze_solver.out: *.c ${BMP_LIBRARY}
	gcc ${COMPILER_FLAGS} $^ -o $@
