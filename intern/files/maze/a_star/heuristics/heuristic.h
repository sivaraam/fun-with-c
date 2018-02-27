#ifndef KS_MAZE_SOLVER_A_STAR_HEURISTIC
#define KS_MAZE_SOLVER_A_STAR_HEURISTIC

#include "../../maze_solver.h"
#include "../../maze_solver_helpers.h"

/**
 * Helper functions to generate the different heuristics
 * of the cost to reach the destination from a given pixel
 * in the given maze.
 *
 * The heuristic is given as an array of 'unsigned int' values.
 *
 * End user note on memory management:
 *
 * The end user is responsible for allocating/de-allocating memory
 * required for the heurisitic vector. No explicit memory management
 * is done internally. So, the heuristic vector is expected to have
 * access to enough memory to initialize the values for "every" valid
 * pixel in the maze.
 */

/**
 * For every clear pixels in the maze, initializes the heuristic vector with
 * the manhattan distance to the destination.
 */
void get_manhattan_heuristic(struct maze_image *maze, struct openings *gates,
                             unsigned *heuristic_values);

#endif
