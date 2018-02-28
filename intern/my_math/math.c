#include "math.h"

#define THRESHOLD (1/2048)

inline int math_abs (int val)
{
	return (val < 0) ? -val : val;
}

double math_floor(double m)
{
	int i_m = m;
	if (m >= 0)
	{
		return i_m;
	}
	else
	{
		if (i_m - m > THRESHOLD)
		{
			return i_m - 1;
		}
		else
		{
			return i_m;
		}
	}
}

double math_max(double a, double b) {
	if (a>b)
		return a;
	else
		return b;
}

double math_fabs(double a) {
	if (a < 0.0) {
		return a*(-1.0);
	}
	return a;
}
