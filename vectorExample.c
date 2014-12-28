#include "vector.h"

int main()
{
	int i;
	char c = 'a';

	vector *v = vector_init(9);
	if (v)
	{
		for(i=0; i < 9; i++)
		{
			if (!vector_add_element(v, (void*)(unsigned long) i + 49))
				printf("error while adding element");
		}

        // vector_delete_element_with_value(v, (void*)(unsigned long) 0);

        vector_print_char(v);

        // vector_delete_element_with_value(v, 1);
        vector_delete_element_with_value(v, (void*)(unsigned long) '2');
        vector_delete_element_with_value(v, (void*)(unsigned long) '1');
        vector_delete_element_with_value(v, (void*)(unsigned long) '9');
        vector_print_char(v);
        // vector_delete_element_with_value(v, (void*)(unsigned long) '2');
		vector_free(v);

		v = vector_init(5);
		for(i=0; i < v->count; i++)
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
			printf("%c\n", (int) (unsigned long) v->data[i]);
		}
		vector_free(v);
	}
	return 0;
}
