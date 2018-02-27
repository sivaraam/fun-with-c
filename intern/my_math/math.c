#include "math.h"

#define THRESHOLD (1/2048)

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
