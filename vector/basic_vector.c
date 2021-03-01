#include "basic_vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

size_t BASIC_SPACE = 5;
short ERROR = 0;

// Utils

void copy_element(void* src, void* dst, const size_t element_size);

// define memory num elements from element as 0
void null_elements(void* element, size_t num, 
	const size_t element_size);

// reallocation memory ("empty" memory is 0) and increasing _space
int8_t* expand(struct basic_vector* const this, unsigned int new_space);

// Methods

void* begin(const struct basic_vector* this);

void* end(const struct basic_vector* this);

const void* get(const struct basic_vector* const this,
	const size_t index);

const void* set(struct basic_vector* const this, const size_t index,
	const void* const new_element, const size_t new_element_size);

const void* push_back(struct basic_vector* const this, 
	const void* new_element, size_t new_element_size);

struct basic_vector* resize(struct basic_vector* const this, 
	size_t new_size);

void delete(struct basic_vector* this);

// Realization

void copy_element(void* src, void* dst, const size_t element_size)
{
	int8_t* a = (int8_t*)src;
	int8_t* b = (int8_t*)dst;
	for (size_t j = 0; j < element_size; ++j)
		b[j] = a[j];
}

void null_elements(void* element, size_t num, 
	const size_t element_size)
{
	int8_t* p = (int8_t*)element;
	int8_t* p_end = p + num * element_size;
	for (; p_end - p > 0; ++p)
	{
		*p = 0;
	}
}

int8_t* expand(struct basic_vector* const this, size_t new_space)
{
	this->elements =
		(int8_t*)realloc(this->elements,
			this->_element_size * new_space);
	if (this->elements == NULL)
	{
		return NULL;
	}
	null_elements(this->elements + this->_space * this->_element_size,
		new_space - this->_space, this->_element_size);
	this->_space = new_space;
	return this->elements;
}

void* begin(const struct basic_vector* this)
{
	return (void*)this->elements;
}

void* end(const struct basic_vector* this)
{
	return (void*)(this->elements + this->size * this->_element_size);
}

const void* get(const struct basic_vector* const this, 
	const size_t index)
{
	if (this->size <= index) 
	{
		ERROR = 20;
		return NULL;
	}
	return (void*)(this->elements + this->_element_size * index);
}

const void* set(struct basic_vector* const this, const size_t index, 
	const void* const new_element, const size_t new_element_size)
{
	//new_element_size must be equal to this->_element_size
	if (this->_element_size != new_element_size)
	{
		ERROR = 30;
		return NULL;
	}

	if(index >= this->size)
	{
		ERROR = 21;
		return NULL;
	}
	const int8_t* source = (const int8_t*)new_element;
	int8_t* destination = this->elements + this->_element_size * index;
	copy_element(source, destination, this->_element_size);
	return get(this, index);
}

const void* push_back(struct basic_vector* const this, 
	const void* new_element, size_t new_element_size)
{
	// memory allocation and rewriting _space
	if (this->_space == 0)
	{
		if (expand(this, BASIC_SPACE) == NULL)
		{
			ERROR = 10;
			return NULL;
		}
	}
	else if (this->_space < (this->size + 1))
	{
		if (expand(this, this->_space * 2) == NULL)
		{
			ERROR = 10;
			return NULL;
		}
	}
	// add an element
	this->size++;
	if(set(this, this->size - 1, new_element, new_element_size) == NULL)
	{
		ERROR = 31;
		return NULL;
	}
	return get(this, this->size - 1);
}

struct basic_vector* resize(struct basic_vector* const this, 
	size_t new_size)
{
	if (new_size > this->_space)
	{
		if (expand(this, new_size) == NULL)
		{
			ERROR = 11;
			return NULL;
		}
	}
	else if(new_size < this->size)
	{
		null_elements(this->elements + new_size * this->_element_size, 
			this->size - new_size, this->_element_size);
	}
	this->size = new_size;
	return this;
}

void delete(struct basic_vector* this)
{
	free(this->elements);
	free(this);
}

// Functions

struct basic_vector* concatenate(struct basic_vector* const one,
	struct basic_vector* const other) 
{
	if (one->_element_size != other->_element_size)
	{
		ERROR = 40;
		return NULL;
	}
	struct basic_vector* res = 
		CreateVectorN(one->size + other->size, one->_element_size);
	size_t i = 0;
	for(; i < one->size; ++i)
	{
		copy_element(one->elements + i * one->_element_size,
			res->elements + i * one->_element_size, one->_element_size);
	}
	for (size_t j = 0; j < other->size; ++j)
	{
		copy_element(other->elements + j * other->_element_size,
			res->elements + (i + j) * one->_element_size,
			one->_element_size);
	}
	return res;
}

void sort(struct basic_vector* this, int (*comp)(const void*, const void*))
{
	qsort(this->elements,this->size, 
		this->_element_size, (*comp));
}

//  f: T -> U map(f, l) writes result to existing vector
void map_back(const struct basic_vector* const src,
	void* (*f)(void*),
	struct basic_vector* const dst)
{
	size_t i = 0;
	for(; i < src->size; ++i)
	{
		dst->push_back(dst, f(src->elements + i * src->_element_size), 
			dst->_element_size);
	}
}

// f: T -> U map(f, l) returns new vector
struct basic_vector* map_new(const struct basic_vector* const src,
	void* /*t*/ (*f)(void* /*u*/), const size_t second_element_size)
{
	struct basic_vector* res = CreateVectorN(src->size, second_element_size);
	size_t i = 0;
	for (; i < src->size; ++i)
	{
		res->set(res, i, 
			f(src->elements + i * src->_element_size), second_element_size);
	}
	return res;
}

//where
//  f: T -> Bool map(f, l) writes result to existing vector
// l and dst must have the same _element_size
struct basic_vector* where(const struct basic_vector* const src, bool (*f)(void*),
	struct basic_vector* const dst)
{
	if (dst->_element_size != src->_element_size)
	{
		ERROR = 41;
		return NULL;
	}
	size_t i = 0;
	for (; i < src->size; ++i)
	{
		if(f(src->elements + i * src->_element_size))
		{
			dst->push_back(dst, src->elements + i * src->_element_size, 
				src->_element_size);
		}
	}
	return dst;
}

// Constructors

struct basic_vector* CreateVectorN(const size_t size, 
	const size_t element_size)
{
	struct basic_vector* v = (struct basic_vector*)malloc(
		sizeof(struct basic_vector));
	
	if (v == NULL)
	{
		ERROR = 12;
		return NULL;
	}

	v->_element_size = element_size;
	v->_space = size;

	if (size == 0) 
		v->elements = NULL;
	else
	{
		v->elements = (int8_t*)calloc(element_size, size);
		if (v->elements == NULL)
		{
			ERROR = 12;
			return NULL;
		}
	}

	v->size = size;
	v->begin = begin;
	v->end = end;
	v->get = get;
	v->set = set;
	v->push_back = push_back;
	v->resize = resize;
	v->delete = delete;
	return v;
}

struct basic_vector* CreateVector0(const size_t element_size)
{
	return CreateVectorN(0, element_size);
}

struct basic_vector* clone()
{
	return NULL;
}
/*
struct basic_vector* CreateVectorFromList(size_t element_size)
{
	struct basic_vector* v = 
		(struct basic_vector*)malloc(sizeof(struct basic_vector));

	if (v == NULL) 
		return NULL;
	
	v->_element_size = element_size;
	v->_space = 
	v->elements = NULL;
	v->size = 0;
	v->begin = begin;
	v->end = end;
	v->get = get;
	v->set = set;
	v->push_back = push_back;
	return v;
}
*/

//Tests

void test_constructors()
{
	struct basic_vector* v0 = CreateVector0(sizeof(int));
	assert(v0->_element_size == sizeof(int));
	assert(v0->_space == 0);
	assert(v0->elements == NULL);
	assert(v0->size == 0);

	struct basic_vector* v4 = CreateVectorN(4, 
		sizeof(long long));
	assert(v4->_element_size == sizeof(long long));
	assert(v4->_space == 4);
	assert(v4->elements != NULL);
	assert(v4->size == 4);
	assert(*(v4->elements) == 0);
}

void test_begin_end()
{
	struct basic_vector* v0 = CreateVector0(sizeof(int));
	assert(v0->begin(v0) == v0->end(v0));
	
	struct basic_vector* v50 = 
		CreateVectorN(50, sizeof(char));
	assert(v50->begin(v50) == v50->elements);
	assert((char*)v50->end(v50) - (char*)v50->begin(v50) == 50);
}

void test_set_get()
{
	struct basic_vector* v = 
		CreateVectorN(5, sizeof(size_t));
	assert(get(v, 5) == NULL);
	assert(get(v, 20) == NULL);
	size_t tmp = 123;
	set(v, 1, &tmp, sizeof(size_t));
	assert(*((size_t*)get(v, 1)) == tmp);
	tmp = 6;
	assert(set(v, &tmp, (size_t)1, sizeof(size_t)) == NULL);
	assert(set(v, 1, &tmp, sizeof(char)) == NULL);
}

void test_push_back()
{
	struct basic_vector* v = 
		CreateVectorN(4, sizeof(char));
	char a = 'a';
	push_back(v, &a, sizeof(a));
	assert(v->size == 5);
	assert(*(char*)get(v, 4) == a);
}

void test_resize()
{
	struct basic_vector* v = CreateVector0(sizeof(int));
	v->resize(v, 5);
	assert(*(int*)get(v, 3) == 0);
	assert(v->size == 5);
	int a = 100;
	set(v, 4, &a, sizeof(a));
	v->resize(v, 2);
	v->resize(v, 5);
	assert(*(int*)get(v, 4) == 0);
}

void test_delete()
{
	struct basic_vector* v = CreateVectorN(4, sizeof(char*));
	v->delete(v);
}

void test_concatenate()
{
	struct basic_vector* v1 = CreateVectorN(5, sizeof(char));
	
	struct basic_vector* v2 = CreateVectorN(5, sizeof(int));
	assert(concatenate(v1, v2) == NULL);
	
	struct basic_vector* v3 = CreateVectorN(7, sizeof(char));
	char a = 'a';
	v3->set(v3, 0, &a, sizeof(a));
	char b = 'b';
	v3->set(v1, 0, &b, sizeof(b));
	
	struct basic_vector* v4 = concatenate(v1, v3);
	assert(v1->size == 5);
	assert(v3->size == 7);
	assert(v4->size == 12);
	assert(v4->_element_size == sizeof(char));
	assert(*(char*)v4->get(v4, 0) == b);
	assert(*(char*)v4->get(v4, 5) == a);
}

int comp_int(const int* a, const int* b)
{
	return *a - *b;
}

void test_sort()
{
	struct basic_vector* v = CreateVectorN(5, sizeof(int));
	int a = 5;
	set(v, 0, &a, sizeof(a));
	a = 29;
	set(v, 1, &a, sizeof(a));
	a = 9;
	set(v, 2, &a, sizeof(a));
	a = 50;
	set(v, 3, &a, sizeof(a));
	a = 1;
	set(v, 4, &a, sizeof(a));

	sort(v, &comp_int);
	
	assert(*(int*)get(v, 0) == 1);
	assert(*(int*)get(v, 1) == 5);
	assert(*(int*)get(v, 2) == 9);
	assert(*(int*)get(v, 3) == 29);
	assert(*(int*)get(v, 4) == 50);
}

int* f1(const char* ch)
{
	int* a = malloc(sizeof(int));
	if(*ch == 'b')
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
	for(int i = 0; i < 4; ++i)
	{
		char tmp = 'a' + i;
		set(v1, i, &tmp, sizeof(char));
	}

	struct basic_vector* v2 = CreateVectorN(1, sizeof(int));
	int a = 1;
	set(v2, 0, &a, sizeof(int));
	map_back(v1, f1, v2);

	assert(v2->size == 5);
	assert(*(int*)get(v2, 0) == 1);
	assert(*(int*)get(v2, 1) == 2);
	assert(*(int*)get(v2, 2) == 1);
	assert(*(int*)get(v2, 3) == 2);
	assert(*(int*)get(v2, 4) == 2);
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
		set(v1, i, &tmp, sizeof(int));
	}

	struct basic_vector* v2 = map_new(v1, g1, sizeof(char));
	assert(v2->size == 4);
	assert(*(char*)get(v2, 0) == 'd');
	assert(*(char*)get(v2, 1) == 'e');
	assert(*(char*)get(v2, 2) == 'f');
	assert(*(char*)get(v2, 3) == 'g');
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
		set(v1, i, &i, sizeof(int));
	}

	struct basic_vector* v2 = CreateVectorN(5, sizeof(char));
	assert(where(v1, h1, v2) == NULL);

	struct basic_vector* v3 = CreateVectorN(1, sizeof(int));
	where(v1, h1, v3);
	
	assert(v3->size == 4);
	assert(*(int*)get(v3, 0) == 0);
	//printf("%d", *(int*)get(v3, 1));
	assert(*(int*)get(v3, 1) == 7);
	assert(*(int*)get(v3, 2) == 8);
	assert(*(int*)get(v3, 3) == 9);	
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