#include "malloc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void	usage(void)
{
	fprintf(stderr, "Usage: ./test <mode> [options]\n");
	fprintf(stderr, "	-t: <simple|medium|hard|realloc>\n");
	fprintf(stderr, "	-o: <option>\n");
}

int main(int argc, char **argv)
{
	int	opt;
	enum {
		UNDEFINED,
		SIMPLE,
		MEDIUM,
		HARD,
		REALLOC
	} mode = UNDEFINED;
	int	option = 0;

	while ((opt = getopt(argc, argv, "t:o:")) != -1)
	{
		switch (opt)
		{
			case 't':
				if (!strcmp(optarg, "simple"))
					mode = SIMPLE;
				else if (!strcmp(optarg, "medium"))
					mode = MEDIUM;
				else if (!strcmp(optarg, "hard"))
					mode = HARD;
				else if (!strcmp(optarg, "realloc"))
					mode = REALLOC;
				else
				{
					fprintf(stderr, "Invalid test %s\n", optarg);
					return 1;
				}
				break ;
			case 'o':
				option = atoi(optarg);
				break ;
			default:
				usage();
				return 1;
		}
	}
	if (mode == UNDEFINED)
	{
		usage();
		return 1;
	}

	if (mode == SIMPLE)
	{
		/* argv[1] malloc() and free() */
		size_t	test_array[] = { sizeof(unsigned int), 200, 1000 };
		size_t	test_array_size = sizeof(test_array) / sizeof(test_array[0]);

		size_t	count;
		size_t	i, j;

		count = option <= 0 ? 1 : option;

		char	*tmp[count];

		for (i = 0; i < test_array_size; i ++)
		{
			fprintf(stderr, "testing %zu malloc() of size %zu\n", count, test_array[i]);
			for (j = 0; j < count; j ++)
			{
				//fprintf(stderr, "alloc index %zu\n", j);

				tmp[j] = (char *)malloc(test_array[i]);
				if (!tmp[j])
				{
					fprintf(stderr, "malloc() failed\n");
					printf("FAILED\n");
					return (1);
				}
			}

			show_memory();

			for (j = 0; j < count; j ++)
			{
				//fprintf(stderr, "free index %zu\n", j);
				free(tmp[j]);
			}

			show_memory();

			printf("SUCCESS\n");
		}
	}
	else if (mode == REALLOC)
	{
		// realloc loop TINY to LARGE
#define STR				"hello world!"
#define STR_SIZE		 sizeof("hello world!") - 1

		size_t	size_max = 4096;
		printf("Testing realloc() from %zu to %zu\n", STR_SIZE, size_max);

		size_t	size;
		size_t	nextsize;
		char	*str;

		str = malloc(STR_SIZE + 1);
		if (!str)
		{
			fprintf(stderr, "malloc() failed size %zu\n", size);
			return (1);
		}
		memcpy(str, STR, STR_SIZE);
		str[STR_SIZE] = 0;

		size = STR_SIZE;
		while (size < size_max)
		{
			nextsize = size + STR_SIZE;
			str = realloc(str, nextsize + 1);
			if (!str)
			{
				fprintf(stderr, "malloc() failed size %zu\n", size);
				return (1);
			}
			fprintf(stderr, "memcpy() %s\n", "");
			memcpy(str + size, STR, STR_SIZE);
			fprintf(stderr, "'0' %s\n", "");
			str[nextsize] = 0;
			size = nextsize;

			fprintf(stderr, "str %p %s\n", (void *)str, str);
		}

		free(str);

		printf("SUCCESS\n");
	}

	return (0);
}
