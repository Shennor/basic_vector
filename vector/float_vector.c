#include "float_vector.h"

int float_cmp_for_vector(const float* a, const float* b)
{
	float fa = *a;
	float fb = *b;
	return (fa > fb) - (fa < fb);
}