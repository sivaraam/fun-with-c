#include <stddef.h>
#include "queue.h"

void initialise_bfsfront_queue(struct bfsfront_queue_head *const head)
{
	head->first = NULL;
	head->last = NULL;
}

int bfsfront_insert_elem(struct bfsfront_queue_head *const head, struct bfsfront_queue_elem *const elem)
{
	if (head == NULL || elem == NULL)
	{
		return 1;
	}

	elem->next = NULL;
	if (head->last == NULL)
	{
		head->first = elem;
		head->last = elem;
	}
	else
	{
		head->last->next = elem;
		head->last = elem;
	}

	return 0;
}

struct bfsfront_queue_elem *bfsfront_remove_elem(struct bfsfront_queue_head *const head)
{
	if (head == NULL)
	{
		return NULL;
	}

	// empty list
	if (head->first == NULL)
	{
		return NULL;
	}

	struct bfsfront_queue_elem *const first_elem = head->first;
	head->first = first_elem->next;

	// last node of list
	if (head->last == first_elem)
	{
		head->last = NULL;
	}

	return first_elem;
}

int bfsfront_queue_empty(struct bfsfront_queue_head *const head)
{
	if (head == NULL ||
	    (head->first == NULL && head->last == NULL))
	{
		return 1;
	}

	return 0;
}
