#include "internal_malloc.h"

/*
 ** context
 */
#define BLOCK_COUNT_MIN	100
size_t	context__compute_total_block_size(const size_t map_size)
{
	uint32_t	total_block_size;

	total_block_size = map_size / BLOCK_COUNT_MIN;
	total_block_size |= (total_block_size >> 1);
	total_block_size |= (total_block_size >> 2);
	total_block_size |= (total_block_size >> 4);
	total_block_size |= (total_block_size >> 8);
	total_block_size |= (total_block_size >> 16);
	total_block_size -= (total_block_size >> 1);
	return (total_block_size);
}

#define TINY_MAP_SIZE		(getpagesize() * 2)
#define SMALL_MAP_SIZE		(getpagesize() * 10)
static bool	context__init(t_context *context)
{
	context->total_allocated_memory = 0;
	context->tiny_info.map_free_count = 0;
	context->tiny_info.size = TINY_MAP_SIZE;
	context->tiny_info.block_type = BLOCK_TYPE__TINY;
	context->tiny_info.total_block_size = context__compute_total_block_size(TINY_MAP_SIZE - sizeof(t_map));
	context->tiny_info.user_block_size = context->tiny_info.total_block_size - sizeof(t_block);
	context->tiny_info.block_count_max = TINY_MAP_SIZE / (context->tiny_info.total_block_size);
	context->small_info.map_free_count = 0;
	context->small_info.size = SMALL_MAP_SIZE;
	context->small_info.block_type = BLOCK_TYPE__SMALL;
	context->small_info.total_block_size = context__compute_total_block_size(SMALL_MAP_SIZE - sizeof(t_map));
	context->small_info.user_block_size = context->small_info.total_block_size - sizeof(t_block);
	context->small_info.block_count_max = SMALL_MAP_SIZE / (context->small_info.total_block_size);
	context->large_info.map_free_count = 0;
	context->large_info.size = 0;
	context->large_info.block_type = BLOCK_TYPE__LARGE;
	context->large_info.total_block_size = 0;
	context->large_info.user_block_size = 0;
	context->large_info.block_count_max = 0;

	INIT_LIST_HEAD(&context->tiny_list_head);
	INIT_LIST_HEAD(&context->small_list_head);
	INIT_LIST_HEAD(&context->large_list_head);
	return (TRUE);
}

extern bool	get_context(t_context **out_context)
{
	static t_context	context = {
		.is_initialized = FALSE
	};

	*out_context = NULL;
	if (context.is_initialized == FALSE)
	{
		if (!context__init(&context))
			return (FALSE);
		context.is_initialized = TRUE;
	}
	*out_context = &context;
	return (TRUE);
}
