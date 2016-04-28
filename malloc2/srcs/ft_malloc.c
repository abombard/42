#include "internal_malloc.h"
#include <sys/mman.h>

bool		block__get_next_block_free(t_block *old_first_block_free, t_block **out_block)
{
	t_list	*start;
	t_list	*pos;
	t_block	*block;

	*out_block = NULL;
	start = &old_first_block_free->list;
	LIST_FOREACH(start, pos)
	{
		block = CONTAINER_OF(pos, t_block, list);
		if (block->state == BLOCK_STATE__FREE)
		{
			*out_block = block;
			return (TRUE);
		}
	}
	LOG_ERROR("Found no free block %s", "");
	return (FALSE);
}

bool		map__get_next_block_free(t_map *map, t_block **out_block)
{
	t_block	*block_free;

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
	return (TRUE);
}

bool		set_next_block_free(t_list *map_list)
{
	t_map	*map;
	t_block	*block_free;
	t_list	*pos;

	LIST_FOREACH(map_list, pos)
	{
		map = CONTAINER_OF(pos, t_map, list);
		if (!map__get_next_block_free(map, &block_free))
			FATAL("map__get_next_block_free() failed on map %p", map);
		map->first_block_free = block_free;
		if (map->first_block_free)
			return (TRUE);
	}
	LOG_ERROR("No free map found %s", "");
	return (FALSE);
}

bool		block_retrieve(t_list *map_list_head, t_block **out_block)
{
	t_list	*pos;
	t_map	*map;

	*out_block = NULL;
	LIST_FOREACH(map_list_head, pos)
	{
		map = CONTAINER_OF(pos, t_map, list);
		if (map->block_count < map->block_count_max)
		{
			if (map->first_block_free == NULL)
				FATAL("First block free is NULL block_count %zu max %zu", map->block_count, map->block_count_max);

			/* Get first block free */
			*out_block = map->first_block_free;
			map->block_count++;

			if (!set_next_block_free(map_list_head))
				FATAL("set_next_block_free() failed %s", "");

			return (TRUE);
		}
	}

	LOG_ERROR("All Map are full %s", "");
	return (FALSE);
}

/*
** map create() / destroy()
*/
void	internal_map__init(t_map *map, const size_t block_size)
{
	t_block	*block;
	void	*addr;
	size_t	block_offset;

	INIT_LIST_HEAD(&map->block_list);
	block_offset = sizeof(t_map);
	addr = map;
	addr += block_offset;
	map->first_block_free = (t_block *)addr;
	while (block_offset < map->size)
	{
		block = (t_block *)(addr);
		block->type = map->block_type;
		block->size = 0;
		block->state = BLOCK_STATE__FREE;
		block->map_addr = map;
		list_add_tail(&block->list, &map->block_list);
		block_offset += block_size;
		addr += block_size;
	}
}

t_map	*map__create(const size_t map_size,
					 const t_block_type block_type,
					 const size_t block_count_max,
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
	map->block_count_max = block_count_max;

	INIT_LIST_HEAD(&map->list);
	internal_map__init(map, block_size);
	return (map);
}

bool	map__destroy(t_map *map)
{
	list_del(&map->list);
	if (munmap(map, map->size) != 0)
	{
		LOG_ERROR("munmap() failed addr %p", (void *)map);
		return (FALSE);
	}
	return (TRUE);
}

/*
** context
*/
#define BLOCK_COUNT_MAX	100
bool	context__init(t_context *context)
{
	int		pagesize;
	size_t	map_size;
	t_map	*tiny;
	t_map	*small;

	pagesize = getpagesize();
	if (pagesize <= 0)
		FATAL("getpagesize() returned %d", pagesize);

	map_size = (pagesize * 5);
	context->tiny_block_size = (map_size - sizeof(t_map)) / BLOCK_COUNT_MAX;

	tiny = map__create(map_size, BLOCK_TYPE__TINY, BLOCK_COUNT_MAX, context->tiny_block_size);
	if (!tiny)
		FATAL("map__create() failed on tiny %s", "");
	INIT_LIST_HEAD(&context->tiny);
	list_add_tail(&tiny->list, &context->tiny);

	map_size = (pagesize * 20);
	context->small_block_size = (map_size - sizeof(t_map)) / BLOCK_COUNT_MAX;

	small = map__create(map_size, BLOCK_TYPE__SMALL, BLOCK_COUNT_MAX, context->small_block_size);
	if (!small)
		FATAL("map__create() failed on small %s", "");
	INIT_LIST_HEAD(&context->small);
	list_add_tail(&small->list, &context->tiny);

	INIT_LIST_HEAD(&context->large);
	return (TRUE);
}

void	*ft_malloc(size_t size)
{
	static t_context	context = {
		.is_initialized = FALSE
	};
	void				*block;
	void				*area;

	if (!size)
		return (NULL);
	if (context.is_initialized == FALSE)
	{
		if (context__init(&context))
			context.is_initialized = TRUE;
		fprintf(stderr, "context initialized\n");
	}

	if (size <= context.tiny_block_size)
	{
		ASSERT(block_retrieve(&context.tiny, (t_block **)&block));
	}
	else if (size <= context.small_block_size)
	{
		ASSERT(block_retrieve(&context.small, (t_block **)&block));
	}
	else
	{
		LOG_ERROR("Large not implemeted %s", "");
		return (NULL);
	}

	area = block + sizeof(t_block);

	LOG_DEBUG("out_area %p", (void *)area);
	return (area);
}
