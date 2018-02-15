#ifndef KS_SUDOKE_SOLVER_NAKED_SINGLE_QUEUE
#define KS_SUDOKE_SOLVER_NAKED_SINGLE_QUEUE

#include <stddef.h>
#include <sys/queue.h>

/**
  * The tail-queue that holds the "naked single" moves to be done.
  */
struct naked_single
{
	size_t row, col;
	STAILQ_ENTRY(naked_single) entries;     /* Tail queue. */
};

/**
  * Initialises the head of the naked single queue.
  * It is a singleton function i.e., initialisation is done only once.
  * Redundant calls have no effect.
  */
void initialise_naked_single_queue(void);

/**
  * Inserts a "naked single" move (identified by [row, col]) into the tail-queue.
  */
void insert_naked_single(size_t row, size_t col);

/**
  * Returns 'true' when there are naked single moves available.
  * Else returns 'false'.
  */
int is_naked_single_available(void);

/**
  * Returns the first node in the naked single queue.
  */
struct naked_single *get_first_naked_single(void);

/**
  * Prints the nodes in the naked single queue.
  */
void print_naked_singles(void);

/**
  * Remove the first node in the naked single queue.
  * Memory for the node is not free()d automatically. It must
  * be done manually by obtaining the node using 'get_first_naked_single'
  */
void remove_first_naked_single(void);

#endif
