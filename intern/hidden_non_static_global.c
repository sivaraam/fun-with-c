#include <stdio.h>

//static int c; not possible
int c;

int main() {
		{
				int c = 0;
				// now, c shadows ::c. just re-declare ::c in a
				// nested scope:
				{
						extern int c;
						c = 1;
				}
				// outputs 0
				printf("%d\n", c);
		}
		// outputs 1
		printf("%d\n", c);
		return 0;
}
