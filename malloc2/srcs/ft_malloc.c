#include "internal_malloc.h"
#include <sys/mman.h>

bool		block__get_next_block_free(t_block *old_first_block_free, t_block **out_block)
{
	t_list	*start;
	t_list	*pos;
	t_block	*block;

	LOG_DEBUG("in old_first_block_free %p", (void *)old_first_block_free);

	*out_block = NULL;
	start = &old_first_block_free->list;
	LIST_FOREACH(start, pos)
	{
		block = CONTAINER_OF(pos, t_block, list);

		LOG_DEBUG("iter %p", (void *)block);

		if (block->state == BLOCK_STATE__FREE)
		{
			*out_block = block;
			LOG_DEBUG("out %p", (void *)block);
			return (TRUE);
		}
	}
	LOG_ERROR("Found no free block %s", "");
	return (FALSE);
}

bool		map__get_next_block_free(t_map *map, t_block **out_block)
{
	t_block	*block_free;

	LOG_DEBUG("%s", "in");

	*out_block = NULL;
	if (map->block_count < map->block_count_max)
	{
		if (map->first_block_free == NULL)
			FATAL("map->first_block_free %p", (void *)map->first_block_free);
		if (!block__get_next_block_free(map->first_block_free, &block_free))
			FATAL("block__get_next_block_free() failed %s", "");
		if (block_free == NULL)
			FATAL("No block free block_count %zu max %zu", map->block_count, map->block_count_max);
		*out_block = block_free;
	}

	LOG_DEBUG("%s", "out");
	return (TRUE);
}

bool		set_next_block_free(t_list *map_list)
{
	t_map	*map;
	t_block	*block_free;
	t_list	*pos;

	LOG_DEBUG("%s", "in");

	LIST_FOREACH(map_list, pos)
	{
		map = CONTAINER_OF(pos, t_map, list);
		if (!map__get_next_block_free(map, &block_free))
			FATAL("map__get_next_block_free() failed on map %p", map);
		map->first_block_free = block_free;
		if (map->first_block_free)
		{
			LOG_DEBUG("%s", "out");
			return (TRUE);
		}
	}
	LOG_ERROR("No more block free %s", "");
	return (FALSE);
}

/*
** block retrieve() / return ()
*/
bool		block_retrieve(t_map_handle *map_handle, t_block **out_block)
{
	t_list	*pos;
	t_map	*map;
	t_block	*block;

	*out_block = NULL;
	LIST_FOREACH(&map_handle->list_head, pos)
	{
		map = CONTAINER_OF(pos, t_map, list);
		if (map->block_count < map->block_count_max)
			break ;
	}
	if (&map_handle->list_head == pos)
	{
		map = map__create(map_handle->map_size, map_handle->block_type, map_handle->block_size);
		if (map == NULL)
			FATAL("map__create() failed map_size %zu block_type %d block_size %zu", map->size, map->block_type, map->block_size);
		list_add_tail(&map->list, &map_handle->list_head);
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

	LOG_DEBUG("block %p", block);

	*out_block = block;
	return (TRUE);
}

bool		block_return(t_block *block)
{
	t_map	*map;

	LOG_DEBUG("block %p", block);

	if (block->state != BLOCK_STATE__USED || block->size == 0)
	{
		LOG_ERROR("block->state %d block->size %zu", block->state, block->size);
		return (FALSE);
	}
	if (block->type == BLOCK_TYPE__TINY || block->type == BLOCK_TYPE__SMALL)
	{
		block->state = BLOCK_STATE__FREE;
		block->size = 0;
		map = block->map_addr;
		map->block_count--;
		if (map->first_block_free == NULL || (void *)block < (void *)map->first_block_free)
		{
			map->first_block_free = block;
		}
		if (map->block_count == 0)
		{
			if (!map__destroy(map))
				FATAL("map__destroy() failed %s", "");
		}
	}
	else if (block->type == BLOCK_TYPE__LARGE)
	{
		list_del(&block->list);
		if (munmap(block, block->size) != 0)
			FATAL("munmap() failed block %p size %zu", (void *)block, block->size);
	}
	else
	{
	    LOG_ERROR("block->type %d", block->type);
		return (FALSE);
	}

	return (TRUE);
}

/*
** map create() / destroy()
*/
bool	internal_map__init(t_map *map, const size_t block_size)
{
	t_block	*block;
	size_t	block_offset;

	if (block_size == 0)
		FATAL("block_size %zu", block_size);
	INIT_LIST_HEAD(&map->block_list);
	map->block_count_max = 0;
	block_offset = sizeof(t_map);
	while (block_offset < map->size)
	{
		block = (void *)map + block_offset;
		block->type = map->block_type;
		block->size = 0;
		block->state = BLOCK_STATE__FREE;
		block->map_addr = map;
		list_add_tail(&block->list, &map->block_list);
		block_offset += block_size;
		map->block_count_max++;
	}
	map->first_block_free = (void *)map + sizeof(t_map);
	return (TRUE);
}

t_map	*map__create(const size_t map_size,
					 const t_block_type block_type,
					 const size_t block_size)
{
	t_map	*map;

	map = mmap(0,
			   map_size,
			   PROT_READ | PROT_WRITE,
			   MAP_ANON | MAP_PRIVATE,
			   -1, 0);
	if (map == NULL)
	{
		LOG_ERROR("mmap() failed map_size %zu", map_size);
		return (NULL);
	}
	map->size = map_size;
	map->block_type = block_type;
	map->block_count = 0;
	INIT_LIST_HEAD(&map->list);
	if (!internal_map__init(map, block_size))
		return (NULL);

	LOG_DEBUG("map %p map_size %zu block_type %d block_size %zu", (void *)map, map_size, block_type, block_size);
	return (map);
}

bool	map__destroy(t_map *map)
{
	LOG_DEBUG("map %p map_size %zu block_type %d block_size %zu", (void *)map, map->size, map->block_type, map->block_size);

	list_del(&map->list);
	if (munmap(map, map->size) != 0)
	{
		LOG_ERROR("munmap() failed addr %p size %zu", (void *)map, map->size);
		return (FALSE);
	}
	return (TRUE);
}

/*
** context
*/
#define BLOCK_COUNT_MIN	100
size_t	context__compute_block_size(const size_t map_size)
{
	uint32_t	block_size;

	block_size = (map_size - sizeof(t_map)) / BLOCK_COUNT_MIN;
	block_size |= (block_size >> 1);
	block_size |= (block_size >> 2);
	block_size |= (block_size >> 4);
	block_size |= (block_size >> 8);
	block_size |= (block_size >> 16);
	block_size -= (block_size >> 1);
	return (block_size);
}

bool	context__init(t_context *context)
{
	int		pagesize;

	pagesize = getpagesize();
	if (pagesize <= 0)
		FATAL("getpagesize() returned %d", pagesize);

	context->tiny.block_type = BLOCK_TYPE__TINY;
	context->tiny.map_size = (pagesize * 2);
	context->tiny.block_size = context__compute_block_size(context->tiny.map_size);
	INIT_LIST_HEAD(&context->tiny.list_head);

	context->small.block_type = BLOCK_TYPE__SMALL;
	context->small.map_size = (pagesize * 15);
	context->small.block_size = context__compute_block_size(context->small.map_size);
	INIT_LIST_HEAD(&context->small.list_head);

	INIT_LIST_HEAD(&context->large);
	return (TRUE);
}

#define BLOCK_SIZE__UNDEFINED	(size_t)-1
bool	get_context(t_context **out_context)
{
	static t_context	context = {
		.is_initialized = FALSE
	};

	*out_context = NULL;
	if (context.is_initialized == FALSE)
	{
		if (!context__init(&context))
			return (FALSE);
		LOG_DEBUG("tiny_block_size %zu", context.tiny.block_size);
		LOG_DEBUG("small_block_size %zu", context.small.block_size);
		context.is_initialized = TRUE;
	}
	*out_context = &context;
	return (TRUE);
}

void	*ft_malloc(size_t size)
{
	t_context			*context;
	t_map_handle		*map_handle;
    t_block				*block;
	void				*area;

	if (!size)
		return (NULL);
	if (!get_context(&context))
		return (NULL);

	if (size < context->small.block_size)
	{
		map_handle = (size < context->tiny.block_size) ? &context->tiny : &context->small;
	    if (!block_retrieve(map_handle, &block))
			FATAL("block_retrieve() failed %s", "");
	}
	else
	{
		block = mmap(0,
					 size,
					 PROT_READ | PROT_WRITE,
					 MAP_ANON | MAP_PRIVATE,
					 -1, 0);
		if (block == NULL)
		{
			LOG_ERROR("mmap() failed block_size %zu", size);
			return (NULL);
		}
		list_add_tail(&block->list, &context->large);
		block->type = BLOCK_TYPE__LARGE;
	}

	block->state = BLOCK_STATE__USED;
	block->size = size;

	area = BLOCK_TO_AREA_OFFSET(block);

	LOG_DEBUG("area %p", (void *)area);
	return (area);
}

void	ft_free(void *ptr)
{
	t_block	*block;

	LOG_DEBUG("area %p", (void *)ptr);
	if (ptr == NULL)
		return ;
	block = AREA_TO_BLOCK_OFFSET(ptr);
	if (!block_return(block))
		LOG_ERROR("block_return() failed %s", "");

	LOG_DEBUG("out %s", "");
}

void	*ft_realloc(void *ptr, size_t size)
{
    t_block	*ptr_block;
	t_block	*new_block;
	void	*new_area;

	LOG_DEBUG("area %p new_size %zu", (void *)ptr, size);

	if (ptr == NULL)
	{
		LOG_ERROR("ptr %p", (void *)ptr);
		return (NULL);
	}

	new_area = ft_malloc(size);
	if (new_area == NULL)
	{
		LOG_ERROR("ft_malloc() failed size %zu", size);
		return (NULL);
	}

	ptr_block = AREA_TO_BLOCK_OFFSET(ptr);
	new_block = AREA_TO_BLOCK_OFFSET(new_area);

	LOG_DEBUG("new_block->type %d new_block->size %zu ptr_block->size %zu", new_block->type, new_block->size, ptr_block->size);
	if (new_block->size < ptr_block->size)
	{
		LOG_ERROR("new_block->size %zu ptr_block->size %zu", new_block->size, ptr_block->size);
		return (NULL);
	}

	LOG_DEBUG("Salut salut %s", "");

	ft_memcpy(new_area, ptr, ptr_block->size);

	LOG_DEBUG("memcpy() ok %s", "");

	ft_free(ptr);

	LOG_DEBUG("area %p", (void *)new_area);
	return (new_area);
}
