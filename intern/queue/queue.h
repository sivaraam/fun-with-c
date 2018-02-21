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
 */
void insert_elem(struct queue_head *head, struct queue_elem *elem);

/**
 * Remove the first element from the queue.
 */
struct queue_elem *remove_elem(struct queue_head *head);

/**
 * Returns non-zero value if queue is empty else returns 0.
 */
int queue_empty(struct queue_head *head);

#endif
