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
CHESS_LIBRARIES = ${CHESS_LIBRARY_PREFIX}/chess.c ${CHESS_LIBRARY_PREFIX}/chess_coins.c ${CHESS_LIBRARY_PREFIX}/chess_helpers.c
IMAGE_PATHS = ./square-paths.c

gui-chess.out: chess-gui.c ${CHESS_LIBRARIES} ${IMAGE_PATHS}
	${GCC} ${GTK3_COMPILER_FLAGS} ${COMPILER_FLAGS} -o $@ $^ ${GTK3_LIBRARIES} ${LINKER_FLAGS}
