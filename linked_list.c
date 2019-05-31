#include <stdio.h>
#include <stdlib.h>

struct node
{
	int value;
	struct node *next;
};

struct singly_linked_list
{
	struct node *head;
	struct node *tail;
};

typedef struct singly_linked_list *list;
typedef struct node *node;

/*
 * List and node creation
 */
node new_node(int value)
{
	node new = malloc(sizeof(struct node));

	if (new == NULL)
		return NULL;

	new->value = value;
	new->next = NULL;

	return new;
}

list new_list()
{
	list new_list = malloc(sizeof(struct singly_linked_list));

	if (new_list == NULL)
		return NULL;

	new_list->head = NULL;
	new_list->tail = NULL;

	return new_list;
}

/*
 * Node insertion
 */

/*
 * Insert the node as the last one
 */
int insert_last(list l, int value)
{
	node n;

	if (l == NULL)
		return -1;

	n = new_node(value);

	if (n == NULL)
		return -1;

	if (l->head == NULL)
	{
		l->head = n;
		l->tail = l->head;
	}
	else
	{
		l->tail->next = n;
		l->tail = n;
	}

	return 0;
}

/*
 * List printing
 */
void print_list(list l)
{
	if (l == NULL || l->head == NULL)
		return;

	node curr = l->head;
	do
	{
		printf("%d ", curr->value);

		curr = curr->next;
	} while (curr != NULL);

	printf("\n");
}

/*
 * Delete the list by free()-ing the memory
 * allocated for it
 */
void delete_list(list l)
{
	node curr;

	if (l == NULL)
		return;

	curr = l->head;

	while (curr != NULL)
	{
		node temp = curr;
		curr = curr->next;
		free(temp);
	}

	free(l);
}

int main(void)
{
	list l = new_list();
	int ret = 0;

	if (l == NULL)
	{
		fprintf(stderr, "List creation failed\n");
		return -1;
	}

	if (insert_last(l, 1) < 0)
	{
		fprintf(stderr, "Insert failed\n");
		ret = -1;
		goto cleanup;
	}

	if (insert_last(l, 2) < 0)
	{
		fprintf(stderr, "Insert failed\n");
		ret = -1;
		goto cleanup;
	}

	print_list(l);

cleanup:
	delete_list(l);
	return ret;
}
