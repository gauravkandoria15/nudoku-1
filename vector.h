#ifndef vector_c
#define vector_c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MULTIPLIER 2

typedef struct vector {
	int size;
	int count;
	void** data;
} vector;

vector* vector_init(int size);
bool vector_add_element(vector* v, void *element);
void vector_delete_element(vector* v, int index);
void vector_free(vector *v);
bool vector_delete_element_with_value(vector* v, void* value);
void vector_print_char(vector* v);
void vector_print_int(vector* v);
vector* vector_copy(vector* v);

#endif // vector_c
