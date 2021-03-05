// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "basic_vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

size_t BASIC_SPACE = 5;
// 4 10
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
	int8_t* tmp_p = (int8_t*)realloc(this->elements,
			this->_element_size * new_space);
	if (tmp_p == NULL)
	{
		return NULL;
	}
	this->elements = tmp_p;
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
		this->_element_size, comp);
}

//  f: T -> U map(f, l) writes result to existing vector
// dst must have _element_size equal to U
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
	const void* /*t*/ (*f)(const void* /*u*/), const size_t second_element_size)
{
	struct basic_vector* res = CreateVectorN(src->size, second_element_size);
	size_t i = 0;
	for (; i < src->size; ++i)
	{
		res->set(res, i, 
			f(get(src, i)), second_element_size);
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
			free(v);
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