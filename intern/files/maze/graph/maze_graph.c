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

	static const unsigned increment = 2;

	// re-allocate when necessary
	if (n->adjlist.num % 2 == 0)
	{
		struct node **temp = realloc(n->adjlist.adjs, (n->adjlist.num+increment)*sizeof(struct node *));
		if (temp == NULL)
		{
			return ERRMEMORY;
		}
		n->adjlist.adjs = temp;
	}

	*(n->adjlist.adjs + n->adjlist.num) = adj;
	n->adjlist.num++;

	return 0;
}

int remove_adjacency(struct node *const n, struct node *const adj)
{
	if (n == NULL ||
	    adj == NULL)
	{
		return ERRNULL;
	}

	for (unsigned curr_adj_index=0; curr_adj_index<n->adjlist.num; curr_adj_index++)
	{
		if (*(n->adjlist.adjs + curr_adj_index) == adj)
		{
			for (size_t chunk=n->adjlist.num-1; chunk>curr_adj_index; chunk--)
			{
				*(n->adjlist.adjs + chunk - 1) = *(n->adjlist.adjs + chunk);
			}
			n->adjlist.num--;
			if (n->adjlist.num%2 == 0)
			{
				struct node **temp = realloc(n->adjlist.adjs, (n->adjlist.num)*sizeof(struct node *));
				if (temp == NULL)
				{
					return ERRMEMORY;
				}
				n->adjlist.adjs = temp;
			}
			return 0;
		}
	}

	return ERRNOADJ;
}
