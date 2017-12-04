#include <stdio.h>

int main() {
    int a[15];
    int c[10];
    int d;

    printf("a:\t\t%p\n", a);
    printf("addr(a):\t%p\n", &a);
    printf("c:\t\t%p\n", c);
    printf("addr(c):\t%p\n", &c);
    printf("addr(d):\t%p\n", &d);
}
