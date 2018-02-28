#ifndef KS_MAZE_SOLVER_COMMON
#define KS_MAZE_SOLVER_COMMON

/* low volume debug info */
#define KS_MAZE_SOLVER_DEBUG
#define KS_MAZE_SOLVER_DEBUG_PROGRESS
// #define KS_MAZE_SOLVER_DEBUG_PRINT_SHORTEST_PATH

/* high volume debug info */
// #define KS_MAZE_SOLVER_DEBUG_CREATE_GRAPH
// #define KS_MAZE_SOLVER_DEBUG_INITIALIZE_ADJACENCIES
// #define KS_MAZE_SOLVER_DEBUG_FIND_SHORTEST_PATH
// #define KS_MAZE_SOLVER_DEBUG_MANHATTAN_DISTANCE
// #define KS_MAZE_SOLVER_DEBUG_PRUNE_DEADEND_NODES

struct openings
{
	unsigned start_gate_pixel,
		 end_gate_pixel;
};

#endif
