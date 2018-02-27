#ifndef KS_PRIORITY_QUEUE
#define KS_PRIORITY_QUEUE

/**
 * Error codes
 */
/**
 * Common error codes
 */
#define ERRNULL 1

/**
 * Codes returned by insert function
 */
#define ERRMEMORY 2
#define ERRHEAP 4


/**
 * A simple priority queue implementation using a bianry min-heap.
 * The element with the least value is considered to have higher priority.
 *
 * End user note on memory management:
 *
 * The end user is expected to take care of allocating/de-allocating the memory
 * for the heap and the elements of the heap. No explicit memory management is
 * done internally to allocate memory for the heap or its elements.
 */

/**
 * A heap element.
 *
 * The key is used to specifies the weight of the value,
 * The key is "not" required to be unique.
 *
 * Each key corresponds to a value the weight of which the
 * key represents.
 */
struct heap_elem
{
	unsigned key;
	<TO_FILL> val;
};

/**
 * The data structure that represents a min heap.
 *
 * The min-heap property for every node 'i' other than the root,
 *
 *    A[Parent(i)] <= A[i]
 *
 * is maintained. The smallest node in a min-heap is the root.
 *
 * The elements of the heap are stored in a linear manner indexed
 * from 1.
 *
 * For any element 'i' of the heap:
 *
 *    - The left child (if it exists) is found at position 2*i.
 *    - The right child (if it existst) is found at position 2*i + 1.
 *    - The parent is found at position floor(i/2)
 *
 * Note on running times:
 *
 * For a heap with 'n' elements
 *    - Insertion is expected to take O(lg n)
 *    - Extracting minimum value is also expected to take O(lg n)
 *
 */
struct min_heap
{
	size_t heap_size;
	size_t capacity;
	struct heap_elem **elements;
};

/**
 * Initialise the elements of the given min heap to their defaults.
 *
 * Returns 0 on success or a non-zero value indicating erro on failure.
 */
int initialise_min_heap(struct min_heap *mheap);

/**
 * Insert a node into the given min-heap.
 *
 * Returns 0 on success or a non-zero value indicating the error on failure.
 */
int min_heap_insert(struct min_heap *mheap, struct heap_elem *elem_to_insert);

/**
 * Returns the heap element with the minimum value (key) among all the elements
 * in the given min-heap.
 *
 * Returns a pointer to a valid heap element on success or returns NULL on failure.
 */
struct heap_elem *extract_min(struct min_heap *mheap);

/**
 * Returns a non-zero value if the heap is empty else returns 0.
 */
int min_heap_empty(struct min_heap *mheap);

#endif
