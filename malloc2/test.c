#include "ft_malloc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		/* argv[1] malloc() and free() */
		size_t	test_array[] = { 6, 20, 42 };
		size_t	test_array_size = sizeof(test_array) / sizeof(test_array[0]);

		size_t	count;
		size_t	i, j;

		count = atoi(argv[1]);

		char	*tmp[count];

		for (i = 0; i < test_array_size; i ++)
		{
			printf("testing %zu malloc() of size %zu\n", count, test_array[i]);

			for (j = 0; j < count; j ++)
			{
				tmp[j] = (char *)ft_malloc(test_array[i]);
				if (!tmp[j])
				{
					count = j - 1;
					break ;
				}
			}

			/*
			for (j = 0; j < count; j ++)
			{
				ft_free(tmp[j]);
				}*/

			printf("SUCCESS\n");
		}
	}
	/*
	else
	{
		// realloc loop TINY to LARGE
#define STR						"hello world!"
#define STR_SIZE		 sizeof("hello world!") - 1

		size_t	size_max = 4096;
		printf("Testing realloc() from %zu to %zu\n", STR_SIZE, size_max);

		size_t	size;
		char	*str;

		str = ft_malloc(STR_SIZE + 1);
		if (!str)
		{
			fprintf(stderr, "ft_malloc() failed size %zu\n", size);
			return (1);
		}
		memcpy(str, STR, STR_SIZE);
		str[STR_SIZE] = 0;

		size = STR_SIZE;

		while (size < size_max)
		{
			str = ft_realloc(str, size + STR_SIZE + 1);
			if (!str)
			{
				fprintf(stderr, "ft_malloc() failed size %zu\n", size);
				return (1);
			}
			memcpy(str + size, STR, STR_SIZE);
			size += STR_SIZE;
			str[size] = 0;

			fprintf(stderr, "str %p %s\n", (void *)str, str);
		}

		ft_free(str);

		printf("SUCCESS\n");
	}
	*/
	return (0);
}
