#ifndef KS_QUEUE
#define KS_QUEUE

/**
 * A simple queue implementation that stores some nodes
 */

typedef <TO_FILL> *elem_type;

struct queue_elem
{
	elem_type *elem;
	struct queue_elem *next;
};

struct queue_head
{
	struct queue_elem *first;
	struct queue_elem *last;
};

/**
 * Initialise the head of the queue.
 */
void initialise_queue(struct queue_head *head);

/**
 * Insert element at the end of the queue.
 *
 * Returns 0 on success and 1 on failure.
 */
int insert_elem(struct queue_head *head, struct queue_elem *elem);

/**
 * Remove the first element from the queue.
 *
 * Returns a pointer to a valid 'struct queue_elem' object on success
 * and NULL on failure.
 */
struct queue_elem *remove_elem(struct queue_head *head);

/**
 * Returns non-zero value if queue is empty else returns 0.
 */
int queue_empty(struct queue_head *head);

#endif
