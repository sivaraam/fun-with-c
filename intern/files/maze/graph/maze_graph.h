#ifndef KS_MAZE_SOLVER_GRAPH
#define KS_MAZE_SOLVER_GRAPH

// #define KS_MAZE_SOLVER_GRAPH_DEBUG

#define ERRNULL 1
#define ERRMEMORY  2

enum search_status
{
	NOT_VISITED,
	IN_FRONTIER,
	VISITIED
};

#pragma pack(push, 4)
/**
 * The data structure to store the adjacency list
 * of the graph nodes.
 */
struct adj_list
{
	unsigned num; // number of adjacent nodes
	struct node **adjs;
};

/**
 * The data structure that represents a graph node
 * for the maze solver. Each node is a clear pixel
 * in the maze.
 *
 * Note: Initializing the structure to 0 would possibly have the
 * correct default values for all elements except pixel.
 */
struct node
{
	unsigned pixel;          // the corresponding pixel value for the node
	unsigned src_dist;       // distance of the node from the source node
	unsigned heuristic;      // the heuristic value that approximates cost to reach destination
	enum search_status colour;      // holds the colour of the node during the search
	struct node *pi;         // predecessor of the node in a path
	struct adj_list adjlist; // the adjacency list for this node
};
#pragma pack(pop)

/**
 * Insert adj as an adjacent node of n.
 *
 * Returns 0 on sucess and non-zero value indicating error on failure.
 */
int insert_adjacency(struct node *const n, struct node *const adj);

#endif
