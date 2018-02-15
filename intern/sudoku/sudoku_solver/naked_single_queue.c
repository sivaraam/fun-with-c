#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "naked_single_queue.h"

/**
 * The head of the tail-queue that holds "naked single" moves.
 */
STAILQ_HEAD(slisthead, naked_single) naked_singles_head = STAILQ_HEAD_INITIALIZER(naked_singles_head);

void initialise_naked_single_queue(void)
{
	STAILQ_INIT(&naked_singles_head);
}

/**
  * Inserts a "naked single" move (identified by [row, col]) into the tail-queue.
  */
void insert_naked_single(size_t row, size_t col)
{

#ifdef KS_SUDOKU_DEBUG
	if (row >= TABLE_ORDER_MAX || col >= TABLE_ORDER_MAX)
	{
		fprintf(stderr, "insert_naked_single: invalid insertion\n"
				"row: %zu, col: %zu", row, col);
		exit(EXIT_FAILURE);
	}
#endif

	struct naked_single *n1 = malloc(sizeof(struct naked_single));
	n1->row = row;	n1->col = col;
	STAILQ_INSERT_TAIL(&naked_singles_head, n1, entries);
}

int is_naked_single_available()
{
	return !STAILQ_EMPTY(&naked_singles_head);
}

struct naked_single *get_first_naked_single(void)
{
	return STAILQ_FIRST(&naked_singles_head);
}

void print_naked_singles(void)
{
	struct naked_single *curr = STAILQ_FIRST(&naked_singles_head);
	while (curr != NULL)
	{
		printf("%zu\t%zu\n", curr->row, curr->col);
		curr = STAILQ_NEXT(curr, entries);
	}
}

void remove_first_naked_single()
{
	STAILQ_REMOVE_HEAD(&naked_singles_head, entries);
}
