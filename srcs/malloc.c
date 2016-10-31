#include "internal_malloc.h"

/*
** malloc()
*/
void	*malloc(size_t size)
{
	t_context		*context;
	t_list			*map_list_head;
	t_map_info		*map_info;
	t_block			*block;
	void			*area;

	fprintf(stderr, "Coucouc c mon Malloc\n");

	if (size == 0)
		return (NULL);
	if (!get_context(&context))
		return (NULL);

	if (size < context->small_info.user_block_size)
	{
		if (size < context->tiny_info.user_block_size)
		{
			map_list_head = &context->tiny_list_head;
			map_info = &context->tiny_info;
		}
		else
		{
			map_list_head = &context->small_list_head;
			map_info = &context->small_info;
		}
		block = block_retrieve(map_info, map_list_head);
		if (block == NULL)
		{
			LOG_ERROR("block_retrieve() failed %s", "");
			return (NULL);
		}
	}
	else
	{
		block = internal_mmap(sizeof(t_block) + size);
		if (block == NULL)
		{
			LOG_ERROR("internal_mmap() failed block_size %zu", size);
			return (NULL);
		}
		list_add_tail(&block->list, &context->large_list_head);
		block->type = BLOCK_TYPE__LARGE;
	}
	block->state = BLOCK_STATE__USED;
	block->size = size;
	area = BLOCK_TO_AREA_OFFSET(block);
	return (area);
}


