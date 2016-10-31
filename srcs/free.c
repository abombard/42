#include "internal_malloc.h"

void	free(void *ptr)
{
	t_block	*block;

	//fprintf(stderr, "Coucouc c mon Free");
	if (ptr == NULL)
		return ;
	block = AREA_TO_BLOCK_OFFSET(ptr);
	if (!block_return(block))
		LOG_ERROR("block_return() failed %s", "");
}

