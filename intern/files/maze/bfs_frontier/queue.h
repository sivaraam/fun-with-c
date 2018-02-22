#ifndef KS_MAZE_SOLVER_BFS_FRONTIER_QUEUE
#define KS_MAZE_SOLVER_BFS_FRONTIER_QUEUE

#include "../graph/maze_graph.h"

/**
 * A simple queue implementation that stores the BFS frontier
 * during a BFS search in a graph.
 */

struct bfsfront_queue_elem
{
	struct node *elem;
	struct bfsfront_queue_elem *next;
};

struct bfsfront_queue_head
{
	struct bfsfront_queue_elem *first;
	struct bfsfront_queue_elem *last;
};

/**
 * Initialise the head of the queue.
 */
void initialise_bfsfront_queue(struct bfsfront_queue_head *head);

/**
 * Insert element at the end of the queue.
 */
void bfsfront_insert_elem(struct bfsfront_queue_head *head, struct bfsfront_queue_elem *elem);

/**
 * Remove the first element from the queue.
 */
struct bfsfront_queue_elem *bfsfront_remove_elem(struct bfsfront_queue_head *head);

/**
 * Returns non-zero value if queue is empty else returns 0.
 */
int bfsfront_queue_empty(struct bfsfront_queue_head *head);

#endif
