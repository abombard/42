#include "internal_malloc.h"

/*
** calloc()
*/
void	*calloc(size_t nmemb, size_t size)
{
	size_t	alloc_size;
	void	*alloc;

	alloc_size = nmemb * size;
	if (alloc_size == 0)
		return (NULL);
	alloc = malloc(alloc_size);
	if (alloc == NULL)
		return (NULL);
	ft_bzero(alloc, alloc_size);
	return (alloc);
}

