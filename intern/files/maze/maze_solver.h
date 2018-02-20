#ifndef KS_MAZE_SOLVER
#define KS_MAZE_SOLVER

// Type of each byte in the maze
typedef unsigned char maze_type;

struct maze_image
{
	maze_type *data;
	unsigned width;
	unsigned height;
	unsigned char padding:2;
};

/**
 * Solve the given maze by finding the shortest path from the start to the end.
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
 * Returns the solved maze in the input itself. Setting the path in a distinct color.
 */
void solve_maze(struct maze_image *maze);

#endif
