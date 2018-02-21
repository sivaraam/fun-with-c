#ifndef REFERENCE_QUEUE
#define REFERENCE_QUEUE

/**
 * A simple queue implementation that stores some nodes
 */

typedef char elem_type;

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

void initialise_queue(struct queue_head *head);

// insert element at the end of the queue
void insert_elem(struct queue_head *head, struct queue_elem *elem);

// remove the first element from the queue
struct queue_elem *remove_elem(struct queue_head *head);

// returns non-zero value if queue is empty else returns 0
int queue_empty(struct queue_head *head);

#endif
