#ifndef KS_MAZE_SOLVER_GRAPH_BRIDGE
#define KS_MAZE_SOLVER_GRAPH_BRIDGE

#include "graph/maze_graph.h"

struct node_list
{
	unsigned pixel;
	struct node *pixel_node;
};

/**
 * The head of the node list that holds the (pixel, node) pairs.
 */
extern struct node_list **np_list;

/**
 * One time allocation of memory for the 'np_list'.
 *
 * size - size is generally 1 plus the maximum pixel value that
 *        would be accessed using the np_list.
 *
 *        This is done to give constant time access of nodes
 *        given the pixel.
 *
 * Returns 0 on success and non-zero value in case of an error.
 *
 * Note: In case of an error the memory for already allocated
 * nodes have to be freed by the caller by calling 'delete_np_list()'.
 */
int initialize_np_list(size_t size);

/**
 * Helper function to creates a valid 'struct node' object for the
 * given pixel and initializes its values to the defaults.
 *
 * Returns NULL in case of an error.
 */
struct node *create_node(unsigned pixel);

/**
 * Creates a new 'struct node_list' entry into 'np_list' for the given
 * node and pixel.
 *
 * Returns 0 on success and non-zero value on failure.
 */
int insert_node(unsigned pixel, struct node *const n);

/**
 * Remove the coressponding 'struct node_list' entry from the 'np_list'
 * for the given pixel.
 *
 * Returns 0 on success and non-zero value on failure.
 *
 * TODO: Re-implement 'np_list' as a linked list to achieve this.
 */
int remove_node(unsigned pixel);

/**
 * Free the memory taken up by the 'np_list'.
 */
void delete_np_list(void);

/**
 * Adds 'adj_pixel_node' as an adjacency of 'pixel_node' and
 * vice versa (as the graph is undirected).
 *
 * Returns 0 on success and non-zero value on error.
 */
int add_adjacency(struct node *const pixel_node, struct node *const adj_pixel_node);

#endif
