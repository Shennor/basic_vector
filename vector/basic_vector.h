#pragma once
#include <stdint.h>
#include "stdbool.h"

extern short ERROR;

// Error list:
// 10 - (push_back) memory allocation returned NULL
// 11 - (resize) memory allocation returned NULL
// 12 - (CreateVectorN / CreateVector0) memory allocation returned NULL
// 
// 20 - (get) out of range
// 21 - (set) out of range
// 
// 30 - (set) new_element_size is not equal to this->_element_size
// 31 - (push_back) new_element_size is not equal to this->_element_size
// 
// 40 - (concatenate) _element_size's of vectors are different
// 41 - (where) _element_size's of src and dst are different
//

struct basic_vector
{
	// size of one element (bytes)
	size_t _element_size;
	// max number of elements in allocated memory
	// (not using memory is always defined as 0)
	size_t _space;
	// data
	int8_t* elements;
	// number of elements
	size_t size;

	// pointer to begin
	void* (*begin)(const struct basic_vector* this);
	// pointer to end
	void* (*end)(const struct basic_vector* this);
	// getting element from index read-only
	const void* (*get)(const struct basic_vector* const this, 
		const size_t index);
	// setting element at index
	const void* (*set)(struct basic_vector* const this, const size_t index, 
		const void* const new_element, const size_t new_element_size);
	// adding an element to the end
	const void* (*push_back)(struct basic_vector* const this, 
		const void* new_element, size_t new_element_size);
	// resizing vector
	struct basic_vector* (*resize)(struct basic_vector* const this, 
		size_t new_size);
	// free all memory of struct and make pointer to this struct NULL
	void (*delete)(struct basic_vector* this);
};

// concatenate two vectors of the same type - binary operation
//(_element_size must be the same)
struct basic_vector* concatenate(struct basic_vector* const one,
	struct basic_vector* const other);

// qsort
void sort(struct basic_vector* this, int (*comp)(const void*, const void*));

// map and push_back result to another vector
void map_back(const struct basic_vector* const src,
	void* (*f)(void*),
	struct basic_vector* const dst);

// map and create new vector of results
struct basic_vector* map_new(const struct basic_vector* const src,
	const void* /*t*/ (*f)(const void* /*u*/), const size_t second_element_size);

//  f: T -> Bool map(f, l) where writes result to existing vector
struct basic_vector* where(const struct basic_vector* const src, bool (*f)(void*),
	struct basic_vector* const dst);

// Constructors

// vector with N elements, those memory initialized as 0
struct basic_vector* CreateVectorN(const size_t size,
	const size_t element_size);
// empty vector - neutral element
struct basic_vector* CreateVector0(const size_t element_size);