#include "ft_malloc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		/* argv[1] malloc() and free() */
		size_t	test_array[] = { 42, 500, 1000 };
		size_t	test_array_size = sizeof(test_array) / sizeof(test_array[0]);

		size_t	count;
		size_t	i, j;

		count = atoi(argv[1]);

		char	*tmp[count];

		for (i = 0; i < test_array_size; i ++)
		{
			fprintf(stderr, "testing %zu malloc() of size %zu\n", count, test_array[i]);
			for (j = 0; j < count; j ++)
			{
				//fprintf(stderr, "alloc index %zu\n", j);

				tmp[j] = (char *)ft_malloc(test_array[i]);
				if (!tmp[j])
				{
					fprintf(stderr, "ft_malloc() failed\n");
					printf("FAILED\n");
					return (1);
				}
			}

			for (j = 0; j < count; j ++)
			{
				//fprintf(stderr, "free index %zu\n", j);
				ft_free(tmp[j]);
			}

			printf("SUCCESS\n");
		}
	}
	else
	{
		// realloc loop TINY to LARGE
#define STR						"hello world!"
#define STR_SIZE		 sizeof("hello world!") - 1

		size_t	size_max = 4096;
		printf("Testing realloc() from %zu to %zu\n", STR_SIZE, size_max);

		size_t	size;
		size_t	nextsize;
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
			nextsize = size + STR_SIZE;
			str = ft_realloc(str, nextsize + 1);
			if (!str)
			{
				fprintf(stderr, "ft_malloc() failed size %zu\n", size);
				return (1);
			}
			fprintf(stderr, "memcpy() %s\n", "");
			memcpy(str + size, STR, STR_SIZE);
			fprintf(stderr, "'0' %s\n", "");
			str[nextsize] = 0;
			size = nextsize;

			fprintf(stderr, "str %p %s\n", (void *)str, str);
		}

		ft_free(str);

		printf("SUCCESS\n");
	}

	return (0);
}
