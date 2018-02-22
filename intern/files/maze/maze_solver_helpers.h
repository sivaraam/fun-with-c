#ifndef KS_MAZE_SOLVER_HELPER
#define KS_MAZE_SOLVER_HELPER

#include "maze_solver.h"
#include "shortest_path/queue.h"

/**
 * For the given maze find the start and end gates.
 *
 * Returns a valid 'struct openings' objct on success.
 * In case of failure returns NULL.
 */
struct openings *find_openings(struct maze_image *maze);

#ifdef KS_MAZE_SOLVER_DEBUG
/**
 * Print an ASCII art version of the given maze.
 */
void print_ascii_maze(struct maze_image *maze);
#endif

// FIXME: Think about moving these to imaze_graph_bridge.

/**
 * Create the graph nodes for each clear pixel in the maze image.
 * Also sets the number of clear pixels in the given maze object.
 *
 * Returns 0 on success and non-zero value on error.
 */
int create_graph(struct maze_image *maze);

/**
 * Initialize the adjacency vertices for node in the clear pixel graph.
 *
 * Returns 0 on success and non-zero value on error.
 */
int initialize_adjacencies(struct maze_image *maze, struct openings *o);

/**
 * Find the shortest path from the start gate node to the end gate node
 * in the graph.
 *
 * Returns the head of a queue that holds the path from the source to the
 * destination.
 */
struct sp_queue_head *find_shortest_path(struct openings *o);

/**
 * Free up the memory taken up by the graph and its related structure.
 */
void delete_graph(void);

#endif
