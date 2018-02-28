#ifndef KS_MAZE_SOLVER
#define KS_MAZE_SOLVER

#define CLEAR_PIXEL 0xFF
#define HURDLE_PIXEL 0x00
#define PIXEL_BYTES 3

#define ERROPENINGS 1
#define ERRMEMORY 2
#define ERRSHPATH 4

// Type of each byte in the maze
typedef unsigned char maze_type;

struct maze_image
{
	maze_type *data;
	unsigned pixels;
	unsigned width;
	unsigned height;
	unsigned char padding:2;
};

/**
 * Solve the given maze by finding the shortest path from the start gate to the end gate.
 *
 * maze - a pointer to a 'struct maze_image' object containing the maze extracted from
 *        the BMP image.
 *        The data is expected to contain the padding that would usually be present
 *        in a BMP image.
 *
 *        Colours:
 *           hurdle - black
 *           clear  - white
 *
 *           No other colours should be present.
 *
 * (width, height) - dimension of the BMP image
 *
 * Returns 0 on success or a non-zero value indicating the error on failure.
 * Returns the solved maze in the input itself. Setting the path in a distinct color.
 */
int solve_maze(struct maze_image *const maze);

#endif
