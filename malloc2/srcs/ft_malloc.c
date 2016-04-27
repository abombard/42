#include "internal_malloc.h"
#include <sys/mman.h>

t_block    *block__retrieve(t_list *map_list_head)
{
	t_block	*block_ret;
	t_block	*block;
	t_list	*pos;
	t_map	*map;

	LIST_FOREACH(map_list_head, pos)
	{
		map = CONTAINER_OF(pos, t_map, list);
		if (map->block_count == map->block_count_max)
			continue ;

		if (!map->first_block_free)
		{
			LOG_ERROR("First block free unavailable %s", "");
			return (NULL);
		}

		/* Get first block free */
		block_ret = map->first_block_free;
		map->block_count++;

		/* //TEMP make a function. Find next block free */
		if (map->block_count < map->block_count_max)
		{
			LIST_FOREACH(&block_ret->list, pos)
			{
				block = CONTAINER_OF(pos, t_block, list);
				if (block->state == BLOCK_STATE__FREE)
				{
					map->first_block_free = block;
					break ;
				}
			}
			if (pos == &block_ret->list)
				FATAL("Could not find a free slot %s", "");
			break ;
		}
	}

	if (map_list_head == pos)
	{
		LOG_ERROR("All maps are full %s", "");
		return (NULL);
	}
	return (block);
}

/*
** map create() / destroy()
*/
void	internal_map__init(t_map *map, const size_t block_size)
{
	t_block	*block;
	void	*addr;
	size_t	block_offset;

	fprintf(stderr, "mat__init()\n");

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

	fprintf(stderr, "map__create()\n");

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
	t_block				*block;
	void				*area;

	if (!size)
		return (NULL);
	fprintf(stderr, "ft_malloc()\n");
	if (context.is_initialized == FALSE)
	{
		if (context__init(&context))
			context.is_initialized = TRUE;
	}

	fprintf(stderr, "context initialized\n");

	if (size <= context.tiny_block_size)
	{
		block = block__retrieve(&context.tiny);
	}
	else if (size <= context.small_block_size)
	{
		block = block__retrieve(&context.small);
	}
	else
	{
		LOG_ERROR("Large not implemeted %s", "");
		return (NULL);
	}

	area = block;
	area += sizeof(t_block);
	return (area);
}
