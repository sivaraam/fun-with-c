COMPILER_FLAGS = "-Wall"
COMPILER_FLAGS += "-Wpedantic"
COMPILER_FLAGS += "-Wextra"
LINKER_FLAGS = "-lm"

sudoku_solver: *.c
	gcc ${COMPILER_FLAGS} $^ -o $@ ${LINKER_FLAGS}


