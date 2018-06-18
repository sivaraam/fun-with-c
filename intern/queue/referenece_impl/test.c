#include <stdio.h>
#include <stdlib.h>
#include "reference_queue.h"

int main(void)
{
  struct queue_head *ref_q = malloc(sizeof(struct queue_head));

  printf("Initialising queue ..\n");
  initialise_queue(ref_q);

  for (size_t i=0; i<3; i++)
  {
    struct queue_elem *temp = malloc(sizeof(struct queue_elem));
    temp->elem = malloc(sizeof(char));
    *(temp->elem) = rand()%20;

    printf("Inserting a new element into queue .. %d\n", *(temp->elem));
    insert_elem(ref_q, temp);
  }

  while(!queue_empty(ref_q))
  {
    printf("Trying to remove an element from the queue ..\n");
    struct queue_elem *temp = remove_elem(ref_q);
    printf("%d\n", *(temp->elem));
    free(temp);
  }
}
