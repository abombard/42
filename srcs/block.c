#include "internal_malloc.h"

/*
 ** block retrieve()
 */
t_block		*block_retrieve(t_map_info *map_info, t_list *map_list_head)
{
	t_list	*pos;
	t_map	*map;
	t_block	*block;

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
		{
			LOG_ERROR("map__create() failed %s", "");
			return (NULL);
		}
		list_add_tail(&map->list, map_list_head);
	}
	LIST_FOREACH(&map->block_list, pos)
	{
		block = CONTAINER_OF(pos, t_block, list);
		if (block->state == BLOCK_STATE__FREE)
			break ;
	}
	if (&map->block_list == pos)
	{
		LOG_ERROR("Found no free block %s", "");
		return (NULL);
	}
	map->block_count++;
	return (block);
}

/*
 ** block return()
 */
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
				{
					LOG_ERROR("map__destroy() failed %s", "");
					return (FALSE);
				}
			}
		}
	}
	else if (block->type == BLOCK_TYPE__LARGE)
	{
		list_del(&block->list);
		if (!internal_munmap(block, block->size))
			return (FALSE);
	}
	return (TRUE);
}

