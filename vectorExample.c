#include "vector.h"

int main()
{
	int i;
	char c = 'a';

	vector *v = init_vector(5);
	if (v)
	{
		for(i=0; i < 6; i++)
		{
			if (!vector_add_element(v, (void*)(unsigned long)i))
				printf("error while adding element");
		}

        vector_delete_element_with_value(v, (void*)(unsigned long) 0);

		printf("zahlen ausgeben: \n");
		for(i=0; i < v->count; i++)
		{
			printf("%d\n", (int)v->data[i]);
		}

		vector_free(v);

		v = init_vector(5);
		for(i=0; i < v->size; i++)
		{
			if (!vector_add_element(v, (void*)(unsigned long)c))
				printf("error while adding element");
			c = c + 1;
		}
		if (!vector_add_element(v, (void*)(unsigned long)c))
			printf("error while adding element");

		vector_delete_element(v, 1);
		vector_delete_element(v, v->count+1);

		printf("buchstaben ausgeben: \n");
		for(i=0; i < v->count; i++)
		{
			printf("%c\n", (int)v->data[i]);
		}
		vector_free(v);
	}
	return 0;
}
