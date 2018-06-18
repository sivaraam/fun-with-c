#include <stddef.h>
#include "reference_queue.h"

void initialise_queue(struct queue_head *head)
{
	head->first = NULL;
	head->last = NULL;
}

void insert_elem(struct queue_head *head, struct queue_elem *elem)
{
	elem->next = NULL;
	if (head->first == NULL)
	{
		head->first = elem;
		head->last = elem;
	}
	else
	{
		head->last->next = elem;
		head->last = elem;
	}
}

struct queue_elem *remove_elem(struct queue_head *head)
{
	// empty list
	if (head->first == NULL)
	{
		return NULL;
	}

	struct queue_elem *first_elem = head->first;
	head->first = first_elem->next;

	// last node of list
	if (head->last == first_elem)
	{
		head->last = NULL;
	}

	return first_elem;
}

int queue_empty(struct queue_head *head)
{
	if (head->first == NULL && head->last == NULL)
	{
		return 1;
	}

	return 0;
}
