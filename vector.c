#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include <stdio.h>

void vector_print_int(vector* v) {
	int i;
	for(i=0; i < v->count; i++)
	{
		printf("%d ", (int) (unsigned long) v->data[i]);
	}
	printf("\n");
}

void vector_print_char(vector* v) {
	int i;
	for(i=0; i < v->count; i++)
	{
		printf("%c ", (int) (unsigned long) v->data[i]);
	}
	printf("\n");
}

vector* vector_init(int size)
{
	vector* v = (vector*)malloc (sizeof(vector));
	if (v)
	{
		v->data = (void**)malloc(size * sizeof(void*));
		if (v->data)
		{
			v->count = 0;
			v->size = size;
		}
		else
		{
			free(v);
			v = NULL;
		}
	}

	return v;
}

bool vector_add_element(vector* v, void *element)
{
	if (v->count == v->size)
	{
		int new_size = v->size*MULTIPLIER;
		void* p = (void*)realloc(v->data, new_size * sizeof(void*));
		if (p == NULL)
			return false;
		v->data = p;
		v->size = new_size;
	}
	v->data[v->count] = element;
	v->count++;

	return true;
}

void vector_delete_element(vector* v, int index)
{

	if(index > v->size)
		return;

	v->data[index] = NULL;
	if (index < v->count)
		memmove(&(v->data[index]), &(v->data[index+1]), sizeof(void*) * (v->count - index ));
	v->count--;
}

bool vector_delete_element_with_value(vector* v, void* value){
	int i;
	if (v == NULL)
		return false;

	for (i = 0; i < v->count; i++) {
		if (v->data[i] == value) {
			vector_delete_element(v, i);
			return true;
		}
	}
	return false;
}

vector* vector_copy(vector * v) {
	vector* target = vector_init(v->size);
	int i;
	for (i = 0; i < v->count; i ++) {
		target->data[i] = v->data[i];
	}
	target->count = v->count;
	return target;
}

bool vector_contains_value(vector *v, void* value) {
	int i;
	for (i = 0; i < v->count; i++){
		if (v->data[i] == value)
			return true;
	}
	return false;
}

int vector_find(vector *v, void* value) {
	int i;
	for (i = 0; i < v->count; i++){
		if (v->data[i] == value)
			return i;
	}
	return -1;
}

void vector_free(vector *v)
{
	if(v)
	{
		if (v->data)
		{
			free(v->data);
			v->data = NULL;
		}
		free(v);
		v = NULL;
	}
}

