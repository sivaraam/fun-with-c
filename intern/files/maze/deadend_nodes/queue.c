#include <stddef.h>
#include "queue.h"

void initialise_de_queue(struct de_queue_head *head)
{
	head->first = NULL;
	head->last = NULL;
}

int de_queue_insert_elem(struct de_queue_head *head, struct de_queue_elem *elem)
{
	// sanity check
	if (head == NULL ||
	    elem == NULL)
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

struct de_queue_elem *de_queue_remove_elem(struct de_queue_head *head)
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

	struct de_queue_elem *first_elem = head->first;
	head->first = first_elem->next;

	// last node of list
	if (head->last == first_elem)
	{
		head->last = NULL;
	}

	return first_elem;
}

int de_queue_empty(struct de_queue_head *head)
{
	if (head == NULL ||
	    (head->first == NULL && head->last == NULL))
	{
		return 1;
	}

	return 0;
}
