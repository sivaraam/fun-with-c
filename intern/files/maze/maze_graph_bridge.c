#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include "maze_solver.h"
#include "maze_graph_bridge.h"
#include "graph/maze_graph.h"

#ifdef KS_MAZE_SOLVER_DEBUG
#include <stdio.h>
#endif

/**
 * Initialize the values for the list
 */

struct node_list **np_list = NULL;

unsigned np_list_vals = 0;

/**
 * Values that specify the incremental dynamic allocation units of
 * the np_list entries.
 */
static const unsigned np_list_increment = 10;
static unsigned np_list_curr_size = 0;

/**
 * Re-allocates memory to hold more (pixel, node) entries in 'np_list'.
 * Re-allocation is done in incremenets of 'np_list_increment'.
 *
 * Returns 0 on success and non-zero value on failure.
 */
static int re_allocate_np_list(void)
{
	const unsigned new_np_list_size = np_list_curr_size+np_list_increment;
	struct node_list **temp = realloc(np_list, (new_np_list_size)*sizeof(struct node_list*));

	if (temp == NULL)
	{
		return 1;
	}

	np_list = temp;

	for (unsigned new=0; new < np_list_increment; new++)
	{
		*(np_list+np_list_curr_size) = malloc(sizeof(struct node_list));

		if (*(np_list+np_list_curr_size) == NULL)
		{
			return 1;
		}

		// increment the curr size as soon as you successfully get each block
		// of memory to precisely track the number of blocks in hold.
		np_list_curr_size++;
	}

	return 0;
}

int insert_node(unsigned pixel, struct node *n)
{
	if (np_list_vals == np_list_curr_size)
	{
		if (re_allocate_np_list())
		{
			return 1;
		}
	}

	(*(np_list+np_list_vals))->pixel = pixel;
	(*(np_list+np_list_vals))->pixel_node = n;
	np_list_vals++;
	return 0;
}

struct node *create_node(unsigned pixel)
{
	struct node *n = calloc(1, sizeof(struct node));

	if (n == NULL)
	{
		return NULL;
	}

	n->pixel = pixel;
	n->adjlist.adjs = NULL; // possibly not necessary as calloc is used, just in case
	n->pi = NULL;

	return n;
}

struct node *get_node(unsigned pixel)
{

	for (unsigned clear=0; clear<np_list_vals; clear++)
	{
		if ( (*(np_list+clear))->pixel == pixel)
		{
			return (*(np_list+clear))->pixel_node;
		}

#ifdef KS_MAZE_SOLVER_DEBUG_GET_NODE
		printf("Found node for pixel: %u at position: %u\n", pixel, clear);
#endif
	}

	return NULL;
}

int add_adjacency(struct node *pixel_node, struct node *adj_pixel_node)
{
	return insert_adjacency(pixel_node, adj_pixel_node);
}

void delete_np_list(void)
{
	// free every node_list entry in hold
	for (unsigned curr=0;curr<np_list_curr_size; curr++)
	{
		free(*(np_list+curr));
		*(np_list+curr) = NULL;
	}

	// free the list itself
	free(np_list);
	np_list = NULL;
}
