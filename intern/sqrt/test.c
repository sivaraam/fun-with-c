#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sqrt.h"

int main(void) {
	double user_input;
	int scanf_ret;

	do
	{
		printf("Enter a non-negative value: ");
		scanf_ret = scanf("%lf", &user_input);

		if (scanf_ret == 0)
		{
			// discard the inputs up and until the first newline
			// (the typical range of invalid inputs)
			int ch = EOF;
			
			do
			{
				ch = getchar();
				if (ch == EOF)
				{
					return 1;
				}
			} while (ch != '\n');
		}
	} while (scanf_ret == 0);

	double sqrt_res = my_sqrt(user_input);
	if (sqrt_res != MY_EINVALID_INPUT) {
		printf("sqrt(%.3f): %.3f\n", user_input, sqrt_res);
		printf("sqrt(%.3f): %.3f (Library func. res)\n", user_input, sqrt(user_input));
	}
	else {
		printf("Enter a valid input!\n");
		exit(EXIT_FAILURE);
	}
}
