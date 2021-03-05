// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <assert.h>
#include <stdlib.h>
#include "basic_vector.h"
#include "test_basic_vector.h"
#include <string.h>
#include "string_vector.h"

void test_constructors()
{
	struct basic_vector* v0 = CreateVector0(sizeof(int));
	assert(v0->_element_size == sizeof(int));
	assert(v0->_space == 0);
	assert(v0->elements == NULL);
	assert(v0->size == 0);
	v0->delete(v0);

	struct basic_vector* v4 = CreateVectorN(4,
		sizeof(long long));
	assert(v4->_element_size == sizeof(long long));
	assert(v4->_space == 4);
	assert(v4->elements != NULL);
	assert(v4->size == 4);
	assert(*(long long*)v4->get(v4, 0) == 0);
	assert(*(long long*)v4->get(v4, 1) == 0);
	assert(*(long long*)v4->get(v4, 2) == 0);
	assert(*(long long*)v4->get(v4, 3) == 0);
	v4->delete(v4);
}

void test_begin_end()
{
	struct basic_vector* v0 = CreateVector0(sizeof(int));
	assert(v0->begin(v0) == v0->end(v0));
	v0->delete(v0);

	struct basic_vector* v50 =
		CreateVectorN(50, sizeof(char));
	assert(v50->begin(v50) == v50->elements);
	assert((char*)v50->end(v50) - (char*)v50->begin(v50) == 50);
	char a = 'f';
	v50->set(v50, 49, &a, sizeof(a));
	assert((char*)v50->end(v50) - 1 == (char*)v50->get(v50, 49));
	assert(*((char*)v50->end(v50) - 1) == 'f');
	v50->delete(v50);
}

void test_set_get()
{
	struct basic_vector* v =
		CreateVectorN(5, sizeof(size_t));
	assert(v->get(v, 5) == NULL);
	assert(ERROR == 20);
	ERROR = 0;
	assert(v->get(v, 20) == NULL);
	assert(ERROR == 20);
	size_t tmp = 123;
	v->set(v, 1, &tmp, sizeof(size_t));
	assert(*((size_t*)v->get(v, 1)) == tmp);
	tmp = 6;
	assert(v->set(v, &tmp, (size_t)1, sizeof(size_t)) == NULL);
	assert(ERROR == 21);
	assert(v->set(v, 1, &tmp, sizeof(char)) == NULL);
	assert(ERROR == 30);
	v->set(v, 4, &tmp, sizeof(size_t));
	assert(*(size_t*)v->get(v, 0) == 0);
	assert(*(size_t*)v->get(v, 1) == 123);
	assert(*(size_t*)v->get(v, 2) == 0);
	assert(*(size_t*)v->get(v, 3) == 0);
	assert(*(size_t*)v->get(v, 4) == 6);
	v->delete(v);

	struct basic_vector* v1 =
		CreateVectorN(3, sizeof(char*));
	char* s1 = "mother";
	char* s2 = "brother";
	char* s3 = "sister";
	v1->set(v1, 0, &s1, sizeof(s1));
	v1->set(v1, 1, &s2, sizeof(s2));
	v1->set(v1, 2, &s3, sizeof(s3));
	assert(strcmp(*(char**)v1->get(v1, 0), "mother") == 0);
	assert(strcmp(*(char**)v1->get(v1, 1), "brother") == 0);
	assert(strcmp(*(char**)v1->get(v1, 2), "sister") == 0);
	assert(*(char**)v1->get(v1, 0) == s1);
	assert(*(char**)v1->get(v1, 1) == s2);
	assert(*(char**)v1->get(v1, 2) == s3);
	v1->delete(v1);
}

void test_push_back()
{
	struct basic_vector* v =
		CreateVectorN(4, sizeof(char));
	char a = 'a';
	v->push_back(v, &a, sizeof(a));
	assert(v->size == 5);
	a = 'b';
	v->push_back(v, &a, sizeof(a));
	assert(v->size == 6);
	assert(*(char*)v->get(v, 0) == '\0');
	assert(*(char*)v->get(v, 1) == '\0');
	assert(*(char*)v->get(v, 2) == '\0');
	assert(*(char*)v->get(v, 3) == '\0');
	assert(*(char*)v->get(v, 4) == 'a');
	assert(*(char*)v->get(v, 5) == 'b');
	v->delete(v);
}

void test_resize()
{
	struct basic_vector* v = CreateVector0(sizeof(int));
	v->resize(v, 5);
	assert(v->size == 5);
	assert(*(int*)v->get(v, 0) == 0);
	assert(*(int*)v->get(v, 1) == 0);
	assert(*(int*)v->get(v, 2) == 0);
	assert(*(int*)v->get(v, 3) == 0);
	assert(*(int*)v->get(v, 4) == 0);
	int a = 100;
	v->set(v, 0, &a, sizeof(a));
	a++;
	v->set(v, 1, &a, sizeof(a));
	a++;
	v->set(v, 2, &a, sizeof(a));
	a++;
	v->set(v, 3, &a, sizeof(a));
	a++;
	v->set(v, 4, &a, sizeof(a));
	v->resize(v, 2);
	assert(v->size == 2);
	v->resize(v, 5);
	assert(*(int*)v->get(v, 0) == 100);
	assert(*(int*)v->get(v, 1) == 101);
	assert(*(int*)v->get(v, 2) == 0);
	assert(*(int*)v->get(v, 3) == 0);
	assert(*(int*)v->get(v, 4) == 0);
	v->delete(v);
}

void test_delete()
{
	struct basic_vector* v = CreateVectorN(4, sizeof(char*));
	v->delete(v);
}

void test_concatenate()
{
	struct basic_vector* v1 = CreateVectorN(3, sizeof(char*));
	struct basic_vector* v2 = CreateVectorN(5, sizeof(int));
	assert(concatenate(v1, v2) == NULL);

	struct basic_vector* v3 = CreateVectorN(4, sizeof(char*));
	char* a = "ab";
	v1->set(v1, 0, &a, sizeof(a));
	a = "ad";
	v1->set(v1, 1, &a, sizeof(a));
	a = "lk";
	v1->set(v1, 2, &a, sizeof(a));

	a = "hy";
	v3->set(v3, 0, &a, sizeof(a));
	a = "rtt";
	v3->set(v3, 1, &a, sizeof(a));
	a = "y";
	v3->set(v3, 2, &a, sizeof(a));
	a = "tw";
	v3->set(v3, 3, &a, sizeof(a));

	struct basic_vector* v4 = concatenate(v1, v3);
	assert(v4->size == 7);
	assert(v4->_element_size == sizeof(char*));
	assert(strcmp(*(char**)v4->get(v4, 0), "ab") == 0);
	assert(strcmp(*(char**)v4->get(v4, 1), "ad") == 0);
	assert(strcmp(*(char**)v4->get(v4, 2), "lk") == 0);
	assert(strcmp(*(char**)v4->get(v4, 3), "hy") == 0);
	assert(strcmp(*(char**)v4->get(v4, 4), "rtt") == 0);
	assert(strcmp(*(char**)v4->get(v4, 5), "y") == 0);
	assert(strcmp(*(char**)v4->get(v4, 6), "tw") == 0);
	v1->delete(v1);
	v2->delete(v2);
	v3->delete(v3);
	v4->delete(v4);
}

int comp_int(const int* a, const int* b)
{
	return *a - *b;
}

void test_sort()
{
	struct basic_vector* v = CreateVectorN(5, sizeof(int));
	int a = 5;
	v->set(v, 0, &a, sizeof(a));
	a = 29;
	v->set(v, 1, &a, sizeof(a));
	a = 9;
	v->set(v, 2, &a, sizeof(a));
	a = 50;
	v->set(v, 3, &a, sizeof(a));
	a = 1;
	v->set(v, 4, &a, sizeof(a));
	sort(v, &comp_int);
	assert(*(int*)v->get(v, 0) == 1);
	assert(*(int*)v->get(v, 1) == 5);
	assert(*(int*)v->get(v, 2) == 9);
	assert(*(int*)v->get(v, 3) == 29);
	assert(*(int*)v->get(v, 4) == 50);
	v->delete(v);

	struct basic_vector* v1 = CreateVectorN(4, sizeof(char*));
	char* a1 = "hello";
	v1->set(v1, 0, &a1, sizeof(a1));
	a1 = "my";
	v1->set(v1, 1, &a1, sizeof(a1));
	a1 = "lovely";
	v1->set(v1, 2, &a1, sizeof(a1));
	a1 = "project!";
	v1->set(v1, 3, &a1, sizeof(a1));
	sort(v1, string_cmp_for_vector);
	assert(strcmp(*(char**)v1->get(v1, 0), "hello") == 0);
	assert(strcmp(*(char**)v1->get(v1, 1), "lovely") == 0);
	assert(strcmp(*(char**)v1->get(v1, 2), "my") == 0);
	assert(strcmp(*(char**)v1->get(v1, 3), "project!") == 0);
	v1->delete(v1);
}

int* f1(const char* ch)
{
	int* a = malloc(sizeof(int));
	if (*ch == 'b')
	{
		*a = 1;
	}
	else
		*a = 2;
	return a;
}

void test_map_back()
{
	struct basic_vector* v1 = CreateVectorN(4, sizeof(char));
	for (int i = 0; i < 4; ++i)
	{
		char tmp = 'a' + i;
		v1->set(v1, i, &tmp, sizeof(char));
	}

	struct basic_vector* v2 = CreateVectorN(1, sizeof(int));
	int a = 1;
	v2->set(v2, 0, &a, sizeof(int));
	map_back(v1, f1, v2);

	assert(v2->size == 5);
	assert(*(int*)v2->get(v2, 0) == 1);
	assert(*(int*)v2->get(v2, 1) == 2);
	assert(*(int*)v2->get(v2, 2) == 1);
	assert(*(int*)v2->get(v2, 3) == 2);
	assert(*(int*)v2->get(v2, 4) == 2);
	v1->delete(v1);
	v2->delete(v2);
}

char* g1(const int* i)
{
	return (char*)i;
}

void test_map_new()
{
	struct basic_vector* v1 = CreateVectorN(4, sizeof(int));
	for (int i = 0; i < 4; ++i)
	{
		char tmp = 100 + i;
		v1->set(v1, i, &tmp, sizeof(int));
	}

	struct basic_vector* v2 = map_new(v1, g1, sizeof(char));
	assert(v2->size == 4);
	assert(*(char*)v2->get(v2, 0) == 'd');
	assert(*(char*)v2->get(v2, 1) == 'e');
	assert(*(char*)v2->get(v2, 2) == 'f');
	assert(*(char*)v2->get(v2, 3) == 'g');
	v1->delete(v1);
	v2->delete(v2);
}

bool h1(const int* a)
{
	return *a > 6;
}

void test_where()
{
	struct basic_vector* v1 = CreateVectorN(10, sizeof(int));
	for (int i = 0; i < v1->size; ++i)
	{
		v1->set(v1, i, &i, sizeof(int));
	}
	struct basic_vector* v2 = CreateVectorN(5, sizeof(char));
	assert(where(v1, h1, v2) == NULL);
	struct basic_vector* v3 = CreateVectorN(1, sizeof(int));
	where(v1, h1, v3);
	assert(v3->size == 4);
	assert(*(int*)v3->get(v3, 0) == 0);
	assert(*(int*)v3->get(v3, 1) == 7);
	assert(*(int*)v3->get(v3, 2) == 8);
	assert(*(int*)v3->get(v3, 3) == 9);
	v1->delete(v1);
	v2->delete(v2);
	v3->delete(v3);
}

void test_all()
{
	test_constructors();
	test_begin_end();
	test_set_get();
	test_push_back();
	test_resize();
	test_delete();
	test_concatenate();
	test_sort();
	test_map_back();
	test_map_new();
	test_where();
}