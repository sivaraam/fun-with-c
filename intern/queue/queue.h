#ifndef KS_QUEUE
#define KS_QUEUE

/**
 * A simple queue implementation that stores some nodes.
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

struct queue_elem
{
	<ELEM_TYPE_TO_FILL> *elem;
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
