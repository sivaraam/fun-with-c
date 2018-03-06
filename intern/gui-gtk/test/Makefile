COMPILER_FLAGS = "-Wall"
COMPILER_FLAGS += "-Wpedantic"
COMPILER_FLAGS += "-Wextra"
COMPILER_FLAGS += "-O3"
COMPILER_FLAGS += "-Wshadow"

GCC = gcc

GTK3_COMPILER_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK3_LIBRARIES = `pkg-config --libs gtk+-3.0`

gui-example.out: example.c
	${GCC} ${GTK3_COMPILER_FLAGS} ${COMPILER_FLAGS} -o $@ $^ ${GTK3_LIBRARIES}

gui-button.out: button.c
	${GCC} ${GTK3_COMPILER_FLAGS} ${COMPILER_FLAGS} -o $@ $^ ${GTK3_LIBRARIES}

gui-switch.out: switch.c
	${GCC} ${GTK3_COMPILER_FLAGS} ${COMPILER_FLAGS} -o $@ $^ ${GTK3_LIBRARIES}

gui-key-press.out: key_press_handler.c
	${GCC} ${GTK3_COMPILER_FLAGS} ${COMPILER_FLAGS} -o $@ $^ ${GTK3_LIBRARIES}

gui-key-press-gobject.out: key_press_handler_gobject.c
	${GCC} ${GTK3_COMPILER_FLAGS} ${COMPILER_FLAGS} -o $@ $^ ${GTK3_LIBRARIES}
