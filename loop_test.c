/* Do the two loops run for the same
   number of times ??? */

#include <stdio.h>

int main(void) {
	unsigned loop_1 = 0, loop_2 = 0;
	const unsigned n = 10;

	for (int i=1; i<=n; i++)
		for (int j=1; j<=i; j++)
			for(int k=1; k<=j; k++)
				loop_1++;

	for (int i=1; i<=n; i++)
		for (int j=i; j<=n; j++)
			for(int k=j; k<=n; k++)
				loop_2++;

	printf("Loop 1 iterated %d times\n", loop_1);
	printf("Loop 2 iterated %d times\n", loop_2);

}
