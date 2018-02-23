#include <stddef.h>
#include <stdlib.h>
#include "maze_graph.h"

int insert_adjacency(struct node *const n, struct node *const adj)
{
	if (n == NULL ||
	    adj == NULL
	)
	{
		return ERRNULL;
	}

	struct adj_list_elem *list_elem = malloc(sizeof(struct adj_list_elem));

	if (list_elem == NULL)
	{
		return ERROOM;
	}

	list_elem->adj = adj;
	list_elem->next = NULL;

	// first adjacency
	if (n->adj_head.first == NULL)
	{
		n->adj_head.first = list_elem;
	}
	else
	{
		list_elem->next = n->adj_head.first;
		n->adj_head.first = list_elem;
	}

	return 0;
}
