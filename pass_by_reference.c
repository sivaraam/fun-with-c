#include <stdio.h>

void swap(int& first, int& second) {  //  Not possible
    int temp = second;
    second = first;
    first = temp;
}

int main(void) {
    int a = 10, b = 20;

    printf("Before swapping: %d %d\n", a, b);

    swap(a, b);

    printf("After swapping: %d %d\n", a, b);
}
