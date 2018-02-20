COMPILER_FLAGS = "-Wall"
COMPILER_FLAGS += "-Wpedantic"
COMPILER_FLAGS += "-Wextra"

line_offset.out: *.c
	gcc ${COMPILER_FLAGS} $^ -o $@

library: file_lines_offset.c
	gcc ${COMPILER_FLAGS} -c $^
