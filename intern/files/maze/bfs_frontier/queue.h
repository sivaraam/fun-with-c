#ifndef KS_MAZE_SOLVER_BFS_FRONTIER_QUEUE
#define KS_MAZE_SOLVER_BFS_FRONTIER_QUEUE

#include "../graph/maze_graph.h"

/**
 * A simple queue implementation that stores the BFS frontier
 * during a BFS search in a graph.
 *
 * Note about memory management:
 * The memory for the queue head object and the
 * queue element objects are expected to be managed
 * by the end user. No explicit memory management is done
 * internally.
 *
 * Therefore, the reposibilities such as obtaining valid
 * memory for new elements (struct bfsfront_queue_elem) to be
 * inserted, freeing memory for the objects that have been
 * removed from the queue, lie in the hands of the end user.
 *
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
void initialise_bfsfront_queue(struct bfsfront_queue_head *const head);

/**
 * Insert element at the end of the queue.
 *
 * Returns 0 on success and non-zero value on error.
 */
int bfsfront_insert_elem(struct bfsfront_queue_head *const head, struct bfsfront_queue_elem *const elem);

/**
 * Remove the first element from the queue.
 *
 * Returns NULL in case of an error (head is NULL (or) queue is empty).
 */
struct bfsfront_queue_elem *bfsfront_remove_elem(struct bfsfront_queue_head *const head);

/**
 * Returns non-zero value if queue is empty else returns 0.
 */
int bfsfront_queue_empty(struct bfsfront_queue_head *const head);

#endif
