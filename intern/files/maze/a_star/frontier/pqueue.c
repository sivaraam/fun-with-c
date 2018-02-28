#include <stdlib.h>
#include <limits.h>
#include "pqueue.h"
#include "../../../../my_math/math.h"

#ifdef KS_PRIORITY_QUEUE_DEBUG
#include <stdio.h>
#endif

#define ERRBADNEWKEY 2

int initialise_min_heap(struct min_heap *const mheap)
{
	if (mheap == NULL)
	{
		return ERRNULL;
	}

	// not required if calloc is used;
	// but you can't be sure
	mheap->capacity = mheap->heap_size = 0;
	mheap->elements = NULL;
	return 0;
}

// macros to find the offsets of the parent/children nodes for a given node offset
#define PARENT(i) ((size_t)(math_floor((double)(i)/2.0)))
#define LEFT(i) (2*(i))
#define RIGHT(i) ( 2 * (i) + 1)

inline static
void swap_nodes(struct heap_elem **const m, struct heap_elem **const n)
{
	struct heap_elem *const swap_temp = *m;
	*m = *n;
	*n = swap_temp;
}

inline static
int check_swap_condition(struct min_heap *const mheap,
                         const unsigned first_offset, const unsigned second_offset)
{
	return (
	        (*(mheap->elements + first_offset))->key < (*(mheap->elements + second_offset))->key ||
	        (
	         (*(mheap->elements + first_offset))->key == (*(mheap->elements + second_offset))->key &&
	         (*(mheap->elements + first_offset))->tie_breaker < (*(mheap->elements + second_offset))->tie_breaker
	        )
	       );
}
/**
 * Decrease the key value of the element in the given offset to the
 * given value and ensure the min-heap property is maintained in the
 * heap.
 *
 * Returns 0 on success and non-zero value on failure.
 */
static int heap_decrease_key(struct min_heap *const mheap, size_t elem_offset, unsigned new_key)
{
#ifdef KS_PRIORITY_QUEUE_DEBUG
	if (mheap == NULL)
	{
		fprintf(stderr, "heap_decrease_key: Invalid heap!\n");
		exit(EXIT_FAILURE);
	}

	if (elem_offset > mheap->heap_size)
	{
		fprintf(stderr, "heap_decrease_key: Invalid element offset!\n");
		exit(EXIT_FAILURE);
	}
#endif

	if (new_key > (*(mheap->elements + elem_offset))->key)
	{

#ifdef KS_PRIORITY_QUEUE_DEBUG
		fprintf(stderr, "heap_decrease_key: Invalid new key: New key is bigger than existing key!\n");
		exit(EXIT_FAILURE);
#endif

		return ERRBADNEWKEY;
	}

	// assign the new key
	(*(mheap->elements + elem_offset))->key = new_key;

	// ensure the heap order property is maintained
	while (elem_offset>1 && check_swap_condition(mheap, elem_offset, PARENT(elem_offset)))
	{
		// swap the current element and its parent
		swap_nodes((mheap->elements + elem_offset), (mheap->elements + PARENT(elem_offset)));

		// continue the process with the parent until the min-heap property is maintained
		elem_offset = PARENT(elem_offset);
	}

	return 0;
}

int min_heap_insert(struct min_heap *const mheap, struct heap_elem *const elem)
{
	if (mheap == NULL || elem == NULL)
	{
		return ERRNULL;
	}

	static const size_t increment = 20;

	// allocate memory to store the element if the heap doesn't have enough
	if (mheap->heap_size == mheap->capacity)
	{
		const size_t new_size = mheap->capacity + increment;

#ifdef KS_PRIORITY_QUEUE_DEBUG
		printf("min_heap_insert: re-allocating memory to store heap elements. new_size: %zu\n", new_size);
#endif

		struct heap_elem **alloc_temp = realloc(mheap->elements, new_size*sizeof(struct heap_elem*));
		if (alloc_temp == NULL)
		{
			return ERRMEMORY;
		}
		mheap->elements = alloc_temp;

		mheap->capacity = new_size;
	}

	mheap->heap_size++;
	(*(mheap->elements + mheap->heap_size)) = elem;

	if (heap_decrease_key(mheap, mheap->heap_size, elem->key))
	{
		return ERRHEAP;
	}
	else
	{
		return 0;
	}
}

/**
 * Ensures the heap order property is maintained for the sub-heap rooted at 'elem_offset'.
 */
void min_heapify(struct min_heap *const mheap, size_t elem_offset)
{

#ifdef KS_PRIORITY_QUEUE_DEBUG
	if (mheap == NULL)
	{
		fprintf(stderr, "heap_decrease_key: Invalid heap!\n");
		exit(EXIT_FAILURE);
	}

	if (elem_offset > mheap->heap_size)
	{
		fprintf(stderr, "heap_decrease_key: Invalid element offset!\n");
		exit(EXIT_FAILURE);
	}
#endif

	size_t smallest = elem_offset, smallest_prev = smallest;

	do
	{
		const size_t left  = LEFT(smallest),
		             right = RIGHT(smallest);
		smallest_prev = smallest;

		// check if the left child is smaller than the current node
		if (left <= mheap->heap_size && check_swap_condition(mheap, left, smallest))
		{
			smallest = left;
		}

		// FIXME: convert the redundant condition into a function or MACRO
		// check if the right child is smaller than the previous smallest
		if (right <= mheap->heap_size && check_swap_condition(mheap, right, smallest))
		{
			smallest = right;
		}

		if (smallest != smallest_prev)
		{
			// swap the current node and the smallest of the left and right children
			swap_nodes((mheap->elements + smallest_prev), (mheap->elements + smallest));
		}

	} while (smallest != smallest_prev);
}

struct heap_elem *extract_min(struct min_heap *const mheap)
{
	if (mheap == NULL)
	{

#ifdef KS_PRIORITY_QUEUE_DEBUG
		printf("extract_min: Invalid heap!\n");
#endif

		return NULL;
	}

	if (mheap->heap_size < 1)
	{

#ifdef KS_PRIORITY_QUEUE_DEBUG
		printf("extract_min: Heap underflow!\n");
#endif

		return NULL;
	}

	struct heap_elem *min = *(mheap->elements + 1);

	// put the last element of the heap as the first element
	*(mheap->elements + 1) = *(mheap->elements + mheap->heap_size);

	mheap->heap_size--;

	// ensure the heap order property is maintained
	min_heapify(mheap, 1);

	return min;
}

inline int min_heap_empty(struct min_heap *const heap)
{
	return (heap->heap_size < 1) ? 1 : 0;
}
