// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "basic_vector.h"
#include "test_basic_vector.h"
#include "string_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "float_vector.h"

int EPS = INT_MAX;

// type:
// 1 - float
// 0 - string

bool ask_vector_type_is_float()
{
	int resp = 0;
	while (1) {
		printf("%s", "Choose one of this numbers:\n");
		printf("%s", "1 - Float\n");
		printf("%s", "2 - String\n");
		scanf_s("%d", &resp);
		switch (resp) {
		case 1: return true;
		case 2: return false;
		default:
			printf("%s", "You've chosen wrong number!"
				"Start again -.- \n");
		}
	}
}
size_t ask_vector_size()
{
	printf("%s", "Okay. Now enter size of vector >> ");
	size_t resp;
	scanf_s("%llu", &resp);
	return resp;
}
bool ask_vector_data_is_automatic()
{
	int resp = 0;
	while (1) {
		printf("%s", "Choose one of this numbers:\n");
		printf("%s", "1 - Fill vector with my data\n");
		printf("%s", "2 - Fill vector with automatic data\n");
		scanf_s("%d", &resp);
		getchar();
		switch (resp) {
		case 1: return false;
		case 2: return true;
		default:
			printf("%s", "You've chosen wrong number!"
				"Start again -.- \n");
		}
	}
}

struct basic_vector* set_automatic_data(struct basic_vector* a, bool is_float)
{
	for (int i = 0; i < a->size; ++i)
	{
		if (is_float)
		{
			float j = (float)(rand() % 10000) / 100;
			a->set(a, i, &j, sizeof(j));
		}
		else
		{
			char* s = malloc(sizeof(char) * 20);
			*(long long*)s = (long long)rand() * (long long)rand();
			s[9] = '\0';
			a->set(a, i, &s, sizeof(s));
		}
	}
	return a;
}
struct basic_vector* set_input_data(struct basic_vector* a, bool is_float)
{
	
	for (size_t i = 0; i < a->size; ++i)
	{
		if (is_float)
		{
			float j;
			scanf_s("%f", &j);
			a->set(a, i, &j, sizeof(j));
		}
		else
		{
			char* s = malloc(sizeof(char) * 20);
			unsigned int count = 0;
			while (1)
			{
				char c = getchar();
				if (c == EOF || c == '\n')
				{
					s[count] = '\0';
					break;
				}
				else
				{
					s[count] = c;
					++count;
				}
			}
			a->set(a, i, &s, sizeof(s));
		}
	}
	return a;
}
void print_vector(struct basic_vector* v, bool is_float)
{
	for (int i = 0; i < v->size; ++i)
	{
		if (is_float)
			printf("%f", *(float*)(v->get(v, i)));
		else
			printf("%s", *(char**)v->get(v, i));
		printf("\n");
	}
}

struct basic_vector* create_vector_float_string(bool is_float)
{
	size_t size = ask_vector_size();
	struct basic_vector* a;
	if (is_float)
		a = CreateVectorN(size, sizeof(float));
	else
		a = CreateVectorN(size, sizeof(char*));
	printf("%s", "Vector is ready.\n");

	if (ask_vector_data_is_automatic())
	{
		set_automatic_data(a, is_float);
		printf("%s", "That's our vector: \n");
		print_vector(a, is_float);
	}
	else
		set_input_data(a, is_float);
	return a;
}

bool float_is_more_than_50(const float* a)
{
	return *a > 50;
}
bool string_begins_with_b(const char* a)
{
	if (strlen(a) == 0)
		return false;
	return a[0] == 'b';
}
const void* float_to_string(const void* a)
{
	char** str = malloc(sizeof(char**));
	*str = calloc(sizeof(char), 2);
	int b = *(float*)a;
	**str = (char)(b + 'a');
	return (void*)str;
}

void sort_interface()
{
	printf("%s", "Let's test sorting.\n");
	struct basic_vector* a;
	while (1) {
		const bool is_float = ask_vector_type_is_float();
		struct basic_vector* a = create_vector_float_string(is_float);
		printf("%s", "So now we gonna sort this poor vector!!!"
			" Get ready, my friend. O-o-oh, okay. We'll start\n");
		if (is_float)
			sort(a, float_cmp_for_vector);
		else
			sort(a, string_cmp_for_vector);
		printf("%s", "Our vector is sorted now *=* \n");
		print_vector(a, is_float);
		a->delete(a);
		while (1) {
			int resp = 0;
			printf("%s", "Print 1 if you want to repeat "
				"sort test and 2 in another way. Next stage is"
				" concatenation testing.\n");
			scanf_s("%d", &resp);
			switch (resp)
			{
			case 2: return;
			case 1: goto label1;
			default:
				printf("%s", "You've chosen wrong number!"
					"Start again -.- \n");
			}
		}
		label1:
		continue;
	}
}

void concatenation_interface()
{
	printf("%s", "Let's test concatenation.\n");
	while(1)
	{
		printf("%s", "We'll create two vectors with "
			"one type of elements\n");
		bool is_float = ask_vector_type_is_float();
		printf("%s", "First vector:\n");
		struct basic_vector* a = create_vector_float_string(is_float);
		struct basic_vector* b = create_vector_float_string(is_float);
		struct basic_vector* c = concatenate(a, b);
		printf("%s", "Now we concatenate two vectors."
			" Here's the result: \n");
		print_vector(c, is_float);
		while (1) {
			int resp = 0;
			printf("%s", "Print 1 if you want to repeat "
				"concatenation test and 2 in another way. Next stage is "
				"map testing.\n");
			scanf_s("%d", &resp);
			switch (resp)
			{
			case 2: return;
			case 1: goto label2;
			default:
				printf("%s", "You've chosen wrong number!"
					"Start again -.- \n");
				goto label2;
			}
		}
		label2:
		continue;
	}
}

void map_interface()
{
	printf("%s", "Let's test map.\n");
	while (1)
	{
		printf("%s", "We have function float -> string."
			" Let's create vector of float\n");
		bool is_float = true;
		struct basic_vector* a = create_vector_float_string(is_float);
		struct basic_vector* b = map_new(a, float_to_string,
			sizeof(char*));
		printf("%s", "Now we created vector-result of map. \n"
			" Here it is: \n");
		print_vector(b, false);
		while (1) {
			int resp = 0;
			printf("%s", "Print 1 if you want to repeat "
				"map test and 2 in another way. Next stage is "
				"where testing.\n");
			scanf_s("%d", &resp);
			switch (resp)
			{
			case 2: return;
			case 1: goto label2;
			default:
				printf("%s", "You've chosen wrong number!"
					"Start again -.- \n");
				goto label2;
			}
		}
	label2:
		continue;
	}
}

void where_interface()
{
	printf("%s", "Let's test where.\n");
	while (1)
	{
		printf("%s", "We have functions (if (float) > 50) and"
			" (if (string)[1] == 'b'). Let's create vector and test where.\n");
		bool is_float = ask_vector_type_is_float();
		struct basic_vector* a = create_vector_float_string(is_float);
		struct basic_vector* b = is_float ? 
			CreateVector0(sizeof(float)) :
			CreateVector0(sizeof(char*));
		is_float ? where(a, float_is_more_than_50, b) :
			where(a, string_begins_with_b, b);
		printf("%s", "Now we created vector-result of where. \n"
			" Here it is: \n");
		print_vector(b, is_float);
		while (1) {
			int resp = 0;
			printf("%s", "Print 1 if you want to repeat "
				"where test and 2 in another way. This is final"
				" stage of program.\n");
			scanf_s("%d", &resp);
			switch (resp)
			{
			case 2: return;
			case 1: goto label2;
			default:
				printf("%s", "You've chosen wrong number!"
					"Start again -.- \n");
			}
		}
	label2:
		continue;
	}
}

int main()
{
	srand(time(0));
	
	// basic tests
	
	test_all();
	
	printf("%s", "Hello! Let's test the structure basic_vector! *.*\n");
	printf("\n");

	// sort
	sort_interface();
	printf("\n");
	
	// concatenation
	concatenation_interface();
	printf("\n");
	
	// map
	map_interface();
	printf("\n");
	
	// where
	where_interface();
	printf("\n");
	
	printf("%s", "We've tested basic functions for basic_vector. Thank you! *+* \n");
	
	return 0;
}