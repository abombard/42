#ifndef MALLOC_H
# define MALLOC_H

#include <unistd.h>

void		free(void *ptr);
void		*malloc(size_t size);
void		*realloc(void *ptr, size_t size);
void		show_memory(void);
void		*calloc(size_t nmemb, size_t size);

#endif
