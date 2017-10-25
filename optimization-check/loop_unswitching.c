#include <stdio.h>
#include <stdlib.h>
#define for_each_string_item(item,list) \
  for (item = (list)->items; \
       item && item < (list)->items + (list)->nr; \
       ++item)

struct string {
    char *items;
    size_t nr;
} *null_string, *valid_string;

void allocate() {
    null_string = (struct string*) malloc(sizeof(struct string));
    valid_string = (struct string*) malloc(sizeof(struct string));

    null_string->items=NULL;
    valid_string->items = (char*) malloc(sizeof(char)*5);
    valid_string->items = "Hello";
    valid_string->nr = 5;
}

int main() {
    allocate();

    char* item;
    for_each_string_item(item, null_string)
       putchar(*item);

    for_each_string_item(item, valid_string)
       putchar(*item);
}

