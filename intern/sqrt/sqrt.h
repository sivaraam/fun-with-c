#ifndef MT_SQRT
#define MT_SQRT
#define MY_EINVALID_INPUT -1111

/**
  * Computes the square root of a non-negative number.
  * Range of input is [0, DBL_MAX]. (TODO: ensure this)
  *
  * Return Value:
  *
  *	non-negative value - square root
  *	negative value     - MY_EINVALID_INPUT
  */
double my_sqrt(double num);

#endif
