#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sqrt.h"

int main(void) {
	double user_input;
	printf("Enter a non-negative value: ");
	scanf("%lf", &user_input);
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
