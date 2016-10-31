#include "malloc.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#define TAB_SIZE 100
int main()
{
	void* mem[TAB_SIZE] = {0};
	int i;
	int j;
	srand(time(NULL));
	while (1)
	{
		i = 0;
		while (i < 1000)
		{
			j = rand() % TAB_SIZE;
			if (mem[j] == NULL)
			{
				size_t size = rand() % 1024;
				mem[j] = malloc(size);
			}
			j = rand() % TAB_SIZE;
			if (mem[j] != NULL)
			{
				free(mem[j]);
				mem[j] = NULL;
			}
			++i;
		}
		usleep(2000);
	}
	return 0;
}
