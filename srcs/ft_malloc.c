#include "internal_malloc.h"
#include <sys/mman.h>

/*
** block retrieve() / return()
*/
bool		block_retrieve(t_map_info *map_info, t_list *map_list_head, t_block **out_block)
{
	t_list	*pos;
	t_map	*map;
	t_block	*block;

	*out_block = NULL;
	LIST_FOREACH(map_list_head, pos)
	{
		map = CONTAINER_OF(pos, t_map, list);
		if (map->block_count < map->info->block_count_max)
			break ;
	}
	if (map_list_head == pos)
	{
		map = map__create(map_info);
		if (map == NULL)
			FATAL("map__create() failed %s", "");
		list_add_tail(&map->list, map_list_head);
	}
	LIST_FOREACH(&map->block_list, pos)
	{
		block = CONTAINER_OF(pos, t_block, list);
		if (block->state == BLOCK_STATE__FREE)
			break ;
	}
	if (&map->block_list == pos)
		FATAL("Found no free block %s", "");
	map->block_count++;
	*out_block = block;
	return (TRUE);
}

bool		block_return(t_block *block)
{
	t_map	*map;

	if (block->state != BLOCK_STATE__USED || block->size == 0 ||
		(block->type != BLOCK_TYPE__TINY && block->type != BLOCK_TYPE__SMALL &&
		 block->type != BLOCK_TYPE__LARGE))
	{
		LOG_ERROR("block->state %d block->size %zu block->type %d",
				  block->state, block->size, block->type);
		return (FALSE);
	}
	if (block->type == BLOCK_TYPE__TINY || block->type == BLOCK_TYPE__SMALL)
	{
		block->state = BLOCK_STATE__FREE;
		block->size = 0;
		map = block->map_addr;
		map->block_count--;
		if (map->block_count == 0)
		{
			map->info->map_free_count++;
			if (map->info->map_free_count > 1)
			{
				if (!map__destroy(map))
					FATAL("map__destroy() failed %s", "");
			}
		}
	}
	else if (block->type == BLOCK_TYPE__LARGE)
	{
		list_del(&block->list);
		if (munmap(block, block->size) != 0)
			FATAL("munmap() failed block %p size %zu", (void *)block, block->size);
	}
	return (TRUE);
}

/*
** map create() / destroy()
*/
bool	internal_map__init(const t_map_info *map_info, t_map *const map)
{
	t_block	*block;
	size_t	block_offset;

	if (!map_info || !map)
		FATAL("map_info %p map %p", (void *)map_info, (void *)map);
	INIT_LIST_HEAD(&map->block_list);
	block_offset = sizeof(t_map);
	while (block_offset < map_info->size)
	{
		block = (void *)map + block_offset;
		block->type = map_info->block_type;
		block->size = 0;
		block->state = BLOCK_STATE__FREE;
		block->map_addr = map;
		list_add_tail(&block->list, &map->block_list);
		block_offset += map_info->total_block_size;
	}
	return (TRUE);
}

t_map	*map__create(const t_map_info *map_info)
{
	t_map	*map;

	map = mmap(0,
			   sizeof(t_block) + map_info->size,
			   PROT_READ | PROT_WRITE,
			   MAP_ANON | MAP_PRIVATE,
			   -1, 0);
	if (map == NULL)
	{
		LOG_ERROR("mmap() failed map_size %zu", map_info->size);
		return (NULL);
	}
	ft_memcpy((void *)&map->info, (void *)&map_info, sizeof(t_map_info *));
	map->block_count = 0;
	INIT_LIST_HEAD(&map->list);
	if (!internal_map__init(map_info, map))
		return (NULL);
	return (map);
}

bool	map__destroy(t_map *map)
{
	list_del(&map->list);
	if (munmap(map, map->info->size) != 0)
	{
		LOG_ERROR("munmap() failed addr %p size %zu", (void *)map, map->info->size);
		return (FALSE);
	}
	return (TRUE);
}

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

bool	get_context(t_context **out_context)
{
	static t_context	context = {
		.is_initialized = FALSE
	};

	*out_context = NULL;
	if (context.is_initialized == FALSE)
	{
		context.tiny_info.map_free_count = 0;
		context.tiny_info.size = TINY_MAP_SIZE;
		context.tiny_info.block_type = BLOCK_TYPE__TINY;
		context.tiny_info.total_block_size = context__compute_total_block_size(TINY_MAP_SIZE - sizeof(t_map));
		context.tiny_info.user_block_size = context.tiny_info.total_block_size - sizeof(t_block);
		context.tiny_info.block_count_max = TINY_MAP_SIZE / (context.tiny_info.total_block_size);
		context.small_info.map_free_count = 0;
		context.small_info.size = SMALL_MAP_SIZE;
		context.small_info.block_type = BLOCK_TYPE__SMALL;
		context.small_info.total_block_size = context__compute_total_block_size(SMALL_MAP_SIZE - sizeof(t_map));
		context.small_info.user_block_size = context.small_info.total_block_size - sizeof(t_block);
		context.small_info.block_count_max = SMALL_MAP_SIZE / (context.small_info.total_block_size);
		context.large_info.map_free_count = 0;
		context.large_info.size = 0;
		context.large_info.block_type = BLOCK_TYPE__LARGE;
		context.large_info.total_block_size = 0;
		context.large_info.user_block_size = 0;
		context.large_info.block_count_max = 0;

		INIT_LIST_HEAD(&context.tiny_list_head);
		INIT_LIST_HEAD(&context.small_list_head);
		INIT_LIST_HEAD(&context.large_list_head);

		context.is_initialized = TRUE;
	}
	*out_context = &context;
	return (TRUE);
}

void	*malloc(size_t size)
{
	t_context		*context;
	t_list			*map_list_head;
	t_map_info		*map_info;
	t_block			*block;
	void			*area;

	//fprintf(stderr, "Coucouc c mon Malloc");

	if (!size)
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
		if (!block_retrieve(map_info, map_list_head, &block))
			FATAL("block_retrieve() failed %s", "");
	}
	else
	{
		block = mmap(0,
					 sizeof(t_block) + size,
					 PROT_READ | PROT_WRITE,
					 MAP_ANON | MAP_PRIVATE,
					 -1, 0);
		if (block == NULL)
		{
			LOG_ERROR("mmap() failed block_size %zu", size);
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

void	*realloc(void *ptr, size_t size)
{
	t_block	*ptr_block;
	t_block	*new_block;
	void	*new_area;

	if (ptr == NULL)
	{
		LOG_ERROR("ptr %p", (void *)ptr);
		return (NULL);
	}
	new_area = malloc(size);
	if (new_area == NULL)
	{
		LOG_ERROR("malloc() failed size %zu", size);
		return (NULL);
	}

	ptr_block = AREA_TO_BLOCK_OFFSET(ptr);
	new_block = AREA_TO_BLOCK_OFFSET(new_area);
	if (new_block->size < ptr_block->size)
	{
		LOG_ERROR("new_block->size %zu ptr_block->size %zu", new_block->size, ptr_block->size);
		return (NULL);
	}
	ft_memcpy(new_area, ptr, ptr_block->size);
	free(ptr);
	return (new_area);
}
