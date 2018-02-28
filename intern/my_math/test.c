#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "math.h"

int main(void)
{
	printf("Floor test:\n");
	printf("math_floor(-2.5): %lf; math_floor(0): %lf; math_floor(3.6): %lf\n",
	       math_floor(-2.5), math_floor(0), math_floor(3.6));

	printf("math_floor(-4): %lf; math_floor(0.8): %lf; math_floor((9/2)): %lf\n",
	       math_floor(-4), math_floor(0.8), math_floor((9/2)));
	printf("\n");

	printf("Absolute value test:\n");
	printf("math_abs(-0): %d; math_abs(45): %d; math_abs(-33): %d\n",
	        math_abs(-0), math_abs(45), math_abs(-33));
	printf("\n");

	printf("Square root test:\n");
	double user_input;
	printf("Enter a non-negative value: ");
	scanf("%lf", &user_input);
	double sqrt_res = math_sqrt(user_input);
	if (sqrt_res != MY_EINVALID_INPUT) {
		printf("sqrt(%.3f): %.3f\n", user_input, sqrt_res);
		printf("sqrt(%.3f): %.3f (Library func. res)\n", user_input, sqrt(user_input));
	}
	else {
		printf("Enter a valid input!\n");
		exit(EXIT_FAILURE);
	}
	printf("\n");

}
