#include <sys/queue.h>

/**
  * The tail-queue that holds the "naked single" moves to be done.
  */
struct naked_single
{
	size_t row, col;
	STAILQ_ENTRY(naked_single) entries;     /* Tail queue. */
};

void initialise_naked_single_queue(void);

void insert_naked_single(size_t row, size_t col);

int is_naked_single_available(void);

struct naked_single *get_first_naked_single(void);

//inline struct naked_single *get_next_naked_single(struct naked_single*);

void print_naked_singles(void);

void remove_first_naked_single(void);

