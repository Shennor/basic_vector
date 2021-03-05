// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "string_vector.h"
#include <string.h>

int string_cmp_for_vector(char** a, char** b)
{
	return strcmp(*a, *b);
}