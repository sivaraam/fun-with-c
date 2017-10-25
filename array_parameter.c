#include <stdio.h>

int foo(int a[10]) {
    printf("%ld", sizeof(a));
    for (int i=0; i<10; i++)
        printf("%d\n", a[i]);
}

int main(void) {
    int a[15] = {15,7,0,9};
    foo(a);
}
