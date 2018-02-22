#ifndef KS_MAZE_SOLVER_COMMON
#define KS_MAZE_SOLVER_COMMON

#define KS_MAZE_SOLVER_DEBUG
// #define KS_MAZE_SOLVER_DEBUG_CREATE_GRAPH
// #define KS_MAZE_SOLVER_DEBUG_GET_NODE
#define KS_MAZE_SOLVER_DEBUG_INITIALIZE_ADJACENCIES

struct openings
{
	unsigned start_gate_pixel,
		 end_gate_pixel;
};

#endif
