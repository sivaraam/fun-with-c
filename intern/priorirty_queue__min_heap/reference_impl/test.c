#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pqueue.h"

int main(void)
{
	struct min_heap *pqueue = malloc(sizeof(struct min_heap));
	if (pqueue == NULL)
	{
		fprintf(stderr, "Could not allocate memory for min heap!\n");
		return 1;
	}

	printf("Priority queue test\n");

	size_t num = 0;
	int scanf_ret = 0;

	do {
		printf("Enter the number of random elements to insert and extract: ");
		scanf_ret = scanf("%zu", &num);
	} while (scanf_ret == 0);

	for (size_t i = 0; i<num; i++)
	{
		struct heap_elem *curr_elem = malloc(sizeof(struct heap_elem));
		if (curr_elem == NULL)
		{
			fprintf(stderr, "Could not allocate memory for element: %zu\n", i);
			return 1;
		}

		curr_elem->key = rand()%UINT_MAX;
		curr_elem->val = i;

		printf("Trying to insert element with key: %u and val: %u\n",
		       curr_elem->key, curr_elem->val);
		if (min_heap_insert(pqueue, curr_elem))
		{
			fprintf(stderr, "Insertion failed for element: %zu\n", i);
			return 1;
		}
		else
		{
			printf("Insertion successful for element: %zu\n", i);
		}
	}

	for (size_t i = 0; i<num; i++)
	{
		struct heap_elem *curr_elem = extract_min(pqueue);
		if (curr_elem == NULL)
		{
			fprintf(stderr, "Extraction of minimum element failed!\n");
			return 1;
		}
		else
		{
			printf("Extracted element with key: %u and val: %u\n",
			       curr_elem->key, curr_elem->val);
		}
		free(curr_elem);
	}

	free(pqueue);
}
