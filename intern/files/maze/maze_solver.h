#ifndef KS_MAZE_SOLVER
#define KS_MAZE_SOLVER

/**
 * Solve the given maze by finding the shortest path from the start to the end.
 *
 * maze - 'unsigned char' data containing the maze extracted from the BMP image.
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
void solve_maze(unsigned char *maze, unsigned width, unsigned height);

#endif
