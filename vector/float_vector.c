// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "float_vector.h"

int float_cmp_for_vector(const float* a, const float* b)
{
	float fa = *a;
	float fb = *b;
	return (fa > fb) - (fa < fb);
}