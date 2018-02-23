#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include "maze_graph_bridge.h"

#ifdef KS_MAZE_SOLVER_DEBUG
#include <stdio.h>
#endif

/**
 * Initialize the values for the list
 */

struct node_list **np_list = NULL;

static unsigned np_list_curr_size = 0;

int initialize_np_list(size_t size)
{
	size_t curr_size = 0;
	np_list = malloc(size * sizeof(struct node_list*));

	if (np_list == NULL)
	{
	    return 1;
	}

	for (; curr_size<size; curr_size++)
	{
		*(np_list+curr_size) = malloc(sizeof(struct node_list));

		if (*(np_list+curr_size) == NULL)
		{
			break;
		}
	}

	np_list_curr_size = size;

	if (curr_size != size)
	{
		return 1;
	}

	return 0;
}

int insert_node(unsigned pixel, struct node *const n)
{
	if (pixel >= np_list_curr_size)
	{
		return 1;
	}

	(*(np_list+pixel))->pixel = pixel;
	(*(np_list+pixel))->pixel_node = n;
	return 0;
}

struct node *create_node(unsigned pixel)
{
	struct node *const n = calloc(1, sizeof(struct node));

	if (n == NULL)
	{
		return NULL;
	}

	n->pixel = pixel;
	n->adjlist.adjs = NULL; // possibly not necessary as calloc is used, just in case
	n->pi = NULL;

	return n;
}

int add_adjacency(struct node *const pixel_node, struct node *const adj_pixel_node)
{
	return insert_adjacency(pixel_node, adj_pixel_node);
}

void delete_np_list(void)
{
	// free every node_list entry in hold
	for (unsigned curr=0;curr<np_list_curr_size; curr++)
	{
		free(*(np_list+curr));
	}

	// free the list itself
	free(np_list);
	np_list = NULL;
}
