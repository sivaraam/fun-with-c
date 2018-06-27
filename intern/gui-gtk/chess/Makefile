COMPILER_FLAGS = "-Wall"
COMPILER_FLAGS += "-Wpedantic"
COMPILER_FLAGS += "-Wextra"
COMPILER_FLAGS += "-O3"
COMPILER_FLAGS += "-Wshadow"
COMPILER_FLAGS += "-Wno-unused-parameter"
LINKER_FLAGS = "-lm"
GCC = gcc

GTK3_COMPILER_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK3_LIBRARIES = `pkg-config --libs gtk+-3.0`

CHESS_LIBRARY_PREFIX = ../../chess_move_validate
IMAGE_PATHS = ./square-paths.c
COMPILER_FLAGS += "-L${CHESS_LIBRARY_PREFIX}"
LINKER_FLAGS += "-lchess-move-validate"
LINKER_FLAGS += "-Wl,-rpath=${CHESS_LIBRARY_PREFIX}"

gui-chess.out: chess-gui.c ${IMAGE_PATHS}
	${GCC} ${GTK3_COMPILER_FLAGS} ${COMPILER_FLAGS} -o $@ $^ ${GTK3_LIBRARIES} ${LINKER_FLAGS}
