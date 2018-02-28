/************   DEV    **********/
//#define MY_SQRT_DEBUG
/************   DEV    **********/
#ifdef MY_SQRT_DEBUG
#include <stdlib.h>
#include <stdio.h>
#endif

#include "math.h"

/*
 * Try to do a binary search to find the square root of the
 * given input (correct with respect to 'delta').
 * Assumes 'num' is valid (i.e.) non-negative and has a square root
 * whose value can be approximated to delta.
 *
 * WARNING: Inputs for which the square root cannot be determined with trade-off
 * of delta lead to an infinite loop.
 */
double bs_sqrt(double num, double delta) {
	// Special case 0 due to limitation of function
	// (The way it handles inpuuut in range (0,1) leads it to give approximate result for 0)
	if (num == 0.0)
		return num;

	double low = 0;
	double high = math_max(num, 1.0); // to handle inputs in range (0,1)
#ifdef MY_SQRT_DEBUG
	unsigned loop_count = 0;
#endif

	double guess = (low+high)/2.0;
	while (math_fabs(guess*guess-(double)num) >= delta && guess <= num) {
#ifdef MY_SQRT_DEBUG
		loop_count++;
		printf("loop count: %u\n", loop_count);
		printf("low: %e\thigh: %e\tfabs(guess*guess-num): %e\t\n", low, high, my_fabs(guess*guess-num));
#endif
		if (guess*guess < num) {
			low = guess;
		} else if (guess*guess > num) {
			high = guess;
		}
#ifdef MY_SQRT_DEBUG
		// This should not happen
		  else {
			fprintf(stderr, "%s", "bs_sqrt: Assumption broken!! 'guess*guess' equals 'num'");
			exit(EXIT_FAILURE);
		}
#endif
		guess = (low+high)/2;
#ifdef MY_SQRT_DEBUG
		printf("guess: %e\n", guess);
		printf("\n");
#endif
	}

	return guess;
}


/* TODO: Handle inputs that for which square root can't be approximated within delta */
double math_sqrt(double num) {
	if (num < 0)
		return MY_EINVALID_INPUT;
	return bs_sqrt(num, 1.0/1024.0);
}

