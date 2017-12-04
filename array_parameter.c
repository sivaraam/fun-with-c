#include <stdio.h>
#include <stdlib.h>

void foo(int a[10]) {
    printf("Address of argument: %p", &a);
    printf("%ld\n", sizeof(a));
    for (int i=0; i<15; i++)
        printf("(address, value): (%p, %d)\n", (a+i), *(a+i));
    printf("\n");
}

void fill_array(int *b, int data[50], size_t len) {
    for(size_t i=0; i<len; i++) {
        b[i] = data[i];
    }
}

int main(void) {
    int a[15] = {15,7,0,9};
    int *b = malloc(sizeof(int)*15);
    int c[4] = {1, 2, 3, 4};
    int d;

    fill_array(b, a, 4u);

    printf("(address, value): (%p, %d)", &d, d);

    printf("sizeof(a): %zu\n", sizeof(a));
    printf("sizeof(b): %zu\n", sizeof(b));
    printf("sizeof(c): %zu\n", sizeof(c));

    foo(a);
    foo(b);
    foo(c);

    a[7] = 100;
    foo(a);
    foo(c);
}
