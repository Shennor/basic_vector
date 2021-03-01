#include "basic_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int EPS = INT_MAX;

// type:
// 1 - float
// 0 - string
void print_vector(struct basic_vector* v, bool type)
{
	for (int i = 0; i < v->size; ++i)
	{
		if (type)
			printf("%f", *(float*)(v->get(v, i)));
		else
			printf("%s", *(char**)v->get(v, i));
			//printf("%s", *(char*)((v->get(v, i))));
		printf("\n");
	}
}

int compare(const void* a, const void* b)
{
	float fa = *(const float*)a;
	float fb = *(const float*)b;
	return (fa > fb) - (fa < fb);
}

bool hgf(const float* a)
{
	return *a > 6;
}

int main()
{
	// basic tests
	
	test_all();
	
	//test sort
	
	printf("%s", "Hello! Let's test the structure basic_vector! *.* "
		"We'll begin with sort testing\n");
	char* choose = "Choose one of this numbers:\n";
	int resp1;
	while (1) {
		printf("%s", choose);
		printf("%s", "1 - Create basic_vector of float's\n");
		printf("%s", "2 - Create basic_vector of string's\n");
		scanf_s("%d", &resp1);
		if (resp1 == 1 || resp1 == 2)
			break;
		else
		{
			printf("%s", "You've chosen wrong number!"
				"Start again -.- \n");
		}
	}

	const bool fl = (resp1 == 1);
	
	printf("%s", "Okay. Now enter size of this vector >> ");
	size_t resp2;
	scanf_s("%llu", &resp2);
	
	struct basic_vector* a;
	if (resp1 == 1)
		a = CreateVectorN(resp2, sizeof(float));
	else
		a = CreateVectorN(resp2, sizeof(char*));
	printf("%s", "Vector is ready.\n");
	
	while (1) {
		printf("%s", choose);
		printf("%s", "1 - Fill vector with my data\n");
		printf("%s", "2 - Fill vector with automatic data\n");
		scanf_s("%d", &resp1);
		if (resp1 == 1 || resp1 == 2)
			break;
		else
		{
			printf("%s", "You've chosen wrong number!"
				"Start again! -.-' \n");
		}
	}
	if (resp1 == 1)
	{
		for (size_t i = 0; i < a->size; ++i)
		{
			if (fl)
			{
				float j;
				scanf_s("%f", &j);
				a->set(a, i, &j, sizeof(j));
			}
			else
			{
				char* s = malloc(sizeof(char) * 20);
 				scanf_s("%s", s);
				a->set(a, i, &s, sizeof(s));
			}
		}
	}
	else
	{
		for (int i = 0; i < a->size; ++i)
		{
			if (fl)
			{
			float j = (float)(rand() % 30) / 10;
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
		printf("%s", "That's our vector: \n");
		print_vector(a, fl);
	}
	printf("%s", "So now we gonna sort this poor vector!!!"
		" Get ready, my friend. O-o-oh, okay. I'm ready. "
		"And you? Tab something to let me know this)\n");
	char* smth = malloc(sizeof(char) * 20);
	scanf_s("%s", &smth);
	if (fl)
		sort(a, compare);
	else
		sort(a, strcmp);
	printf("%s", "Our vector is sorted now *=* \n");
	print_vector(a, fl);

	// test concatenation
	
	printf("%s", "Let's test concatenation. For this we'll make another "
		"vector. Here it is (automaticly genered) (for example size is 6)\n");
	struct basic_vector* b;
	if (resp1 == 1)
		b = CreateVectorN(6, sizeof(float));
	else
		b = CreateVectorN(6, sizeof(char*));
	for (int i = 0; i < b->size; ++i)
	{
		if (fl)
		{
			float j = (float)(rand() % 30) / 10;
			b->set(b, i, &j, sizeof(j));
		}
		else
		{
			char* s = malloc(sizeof(char) * 20);
			*(long long*)s = (long long)rand() * (long long)rand();
			s[9] = '\0';
			b->set(b, i, &s, sizeof(s));
		}
	}
	print_vector(b, fl);
	printf("%s", "Now we concatenate two vectors."
		" Here's the result: \n");
	struct basic_vector* c;
	if (resp1 == 1)
		c = CreateVectorN(resp2, sizeof(float));
	else
		c = CreateVectorN(resp2, sizeof(char*));
	c = concatenate(a, b);
	print_vector(c, fl);

	
	//printf("%s", choose);
	return 0;
}