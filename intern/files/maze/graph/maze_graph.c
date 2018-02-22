#include <stddef.h>
#include <stdlib.h>
#include "maze_graph.h"

int insert_adjacency(struct node *n, struct node *adj)
{
	if (n == NULL)
	{
		return ERRNULL;
	}

	static const unsigned increment = 2;

	// re-allocate when necessary
	if (n->adjlist.num % 2 == 0)
	{
		struct node **temp = realloc(n->adjlist.adjs, (n->adjlist.num+increment)*sizeof(struct node *));
		if (temp == NULL)
		{
			return ERROOM;
		}
		n->adjlist.adjs = temp;
	}

	*(n->adjlist.adjs + n->adjlist.num) = adj;
	n->adjlist.num++;

	return 0;
}
