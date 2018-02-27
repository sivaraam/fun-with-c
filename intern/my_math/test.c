#include <stdio.h>
#include "math.h"

int main(void)
{
	printf("math_floor(-2.5): %lf; math_floor(0): %lf; math_floor(3.6): %lf\n",
	       math_floor(-2.5), math_floor(0), math_floor(3.6));

	printf("math_floor(-4): %lf; math_floor(0.8): %lf; math_floor((9/2)): %lf\n",
	       math_floor(-4), math_floor(0.8), math_floor((9/2)));
}
