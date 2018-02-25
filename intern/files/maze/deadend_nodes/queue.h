#ifndef KS_MAZE_SOLVER_DEADEND_QUEUE
#define KS_MAZE_SOLVER_DEADEND_QUEUE

/**
 * A simple queue implementation to store the list of dead end
 * nodes. Dead end nodes are those nodes which have an adjacency
 * value of 1 in an undirected graph.
 *
 * Note about memory management:
 * The memory for the 'struct queue_head' object and the
 * 'struct queue_elem' objects are expected to be managed
 * by the end user. No explicit memory management is done
 * internally.
 *
 * Therefore, the reposibilities such as obtaining valid
 * memory for new elements (struct queue_elem) to be inserted,
 * freeing memory for the objects that have been removed
 * from the queue, lie in the hands of the end user.
 *
 */

struct de_queue_elem
{
	struct node *elem;
	struct de_queue_elem *next;
};

struct de_queue_head
{
	struct de_queue_elem *first;
	struct de_queue_elem *last;
};

/**
 * Initialise the head of the queue.
 */
void initialise_de_queue(struct de_queue_head *head);

/**
 * Insert element at the end of the queue.
 *
 * Returns 0 on success and 1 on failure.
 */
int de_queue_insert_elem(struct de_queue_head *head, struct de_queue_elem *elem);

/**
 * Remove the first element from the queue.
 *
 * Returns a pointer to a valid 'struct queue_elem' object on success
 * and NULL on failure.
 */
struct de_queue_elem *de_queue_remove_elem(struct de_queue_head *head);

/**
 * Returns non-zero value if queue is empty else returns 0.
 */
int de_queue_empty(struct de_queue_head *head);

#endif
