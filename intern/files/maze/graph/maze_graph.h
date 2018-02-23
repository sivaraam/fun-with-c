#ifndef KS_MAZE_SOLVER_GRAPH
#define KS_MAZE_SOLVER_GRAPH

#define ERRNULL 1
#define ERROOM  2
#define ERRNOADJ 4
enum bfs_colour
{
	WHITE,
	GREY,
	BLACK
};

/**
 * The data structure to store the adjacency list
 * of the graph nodes.
 */
struct adj_list_elem
{
	struct node *adj;
	struct adj_list_elem *next;
};

/**
 * The head of the adjacency list for a node.
 */
struct adj_list_head
{
	struct adj_list_elem *first;
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
	unsigned pixel; // the corresponding pixel value for the node
	struct adj_list_head adj_head; // the adjacency list for this node

	// BFS related data
	enum bfs_colour colour; // holds the colour of the node during the search
	unsigned src_dist; // distance of the node from the source node
	struct node *pi; // predecessor of the node
};

/**
 * Insert adj as an adjacent node of n.
 *
 * Returns 0 on sucess and non-zero value indicating error on failure.
 */
int insert_adjacency(struct node *const n, struct node *const adj);

/**
 * Remove adj from the list of adjacencies of n.
 *
 * Returns 0 on success and non-zero value indicating error on failure.
 */
int remove_adjacency(struct node *const n, struct node *const adj);

#endif
