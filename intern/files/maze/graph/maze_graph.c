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

int remove_adjacency(struct node *const n, struct node *const adj)
{
	if (
		n == NULL ||
		adj == NULL)
	{
		return ERRNULL;
	}

	struct adj_list_elem *curr_adj_elem = n->adj_head.first;

	// empty list
	if (curr_adj_elem == NULL)
	{
		return ERRNOADJ;
	}

	// first element of list
	if (curr_adj_elem->adj == adj)
	{
		n->adj_head.first = NULL;
		free(curr_adj_elem);
		return 0;
	}

	while (curr_adj_elem->next != NULL && curr_adj_elem->next->adj != adj)
	{
		curr_adj_elem = curr_adj_elem->next;
	}

	if (curr_adj_elem->next == NULL)
	{
		return ERRNOADJ;
	}

	struct adj_list_elem *elem_to_free = curr_adj_elem->next;
	curr_adj_elem->next = curr_adj_elem->next->next;
	free(elem_to_free);
	return 0;
}
