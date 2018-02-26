#include <stddef.h>
#include <stdlib.h>
#include "maze_graph.h"

#ifdef KS_MAZE_SOLVER_GRAPH_DEBUG
#include <stdio.h>
#endif

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

#ifdef KS_MAZE_SOLVER_GRAPH_DEBUG
	printf("remove_adjacency: searching for adjacency %u (%p) among %u adjacencies of %u (%p)\n",
	       adj->pixel, (void *) adj, n->adjlist.num, n->pixel, (void *) n);
#endif

	for (unsigned curr_adj_index=0; curr_adj_index<n->adjlist.num; curr_adj_index++)
	{

#ifdef KS_MAZE_SOLVER_GRAPH_DEBUG
		printf("remove_adjacency: Found adjacency %u (%p)\n",
		       (*(n->adjlist.adjs + curr_adj_index))->pixel,
		       (void *) *(n->adjlist.adjs + curr_adj_index));
		fflush(stdout);
#endif

		if (*(n->adjlist.adjs + curr_adj_index) == adj)
		{
			for (size_t chunk=curr_adj_index+1; chunk<n->adjlist.num; chunk++)
			{
				*(n->adjlist.adjs + chunk - 1) = *(n->adjlist.adjs + chunk);
			}
			n->adjlist.num--;
			if (n->adjlist.num%2 == 0)
			{
				struct node **temp = realloc(n->adjlist.adjs, (n->adjlist.num)*sizeof(struct node *));
				if (temp == NULL && n->adjlist.num != 0)
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
