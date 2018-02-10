#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

struct slisthead *headp;                /* Singly-linked List head. */
struct entry {
	size_t row, col;
        STAILQ_ENTRY(entry) entries;     /* Singly-linked List. */
};

int main(void) {
	STAILQ_HEAD(slisthead, entry) head = STAILQ_HEAD_INITIALIZER(head);
	STAILQ_INIT(&head);

	struct entry *n1 = malloc(sizeof(struct entry));
	n1->row = 0;
	n1->col = 4;
	STAILQ_INSERT_HEAD(&head, n1, entries);

	while (!STAILQ_EMPTY(&head)) {           /* List Deletion. */
		struct entry *n2 = STAILQ_FIRST(&head);
		STAILQ_REMOVE_HEAD(&head, entries);
		printf("%zu %zu\n", n2->row, n2->col);
		free(n2);
	}
}
