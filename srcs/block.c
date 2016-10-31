/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 12:58:35 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 13:17:22 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc.h"

/*
** block retrieve
*/

static t_map	*get_map(t_map_info *info, t_list *head)
{
	t_map	*map;
	t_list	*pos;

	pos = head;
	while ((pos = pos->next) && pos != head)
	{
		map = CONTAINER_OF(pos, t_map, list);
		if (map->block_count < map->info->block_count_max)
			break ;
	}
	if (head == pos)
	{
		map = map__create(info);
		if (map == NULL)
		{
			LOG_ERROR("map__create() failed %s", "");
			return (NULL);
		}
		list_add_tail(&map->list, head);
	}
	return (map);
}

extern t_block	*block_retrieve(t_map_info *map_info, t_list *map_list_head)
{
	t_list	*pos;
	t_map	*map;
	t_block	*block;

	map = get_map(map_info, map_list_head);
	if (map == NULL)
		FATAL("get_map failed %s", "");
	pos = &map->block_list;
	while ((pos = pos->next) && pos != &map->block_list)
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
** block return
*/

extern bool		block_return(t_block *block)
{
	t_map	*map;

	if (block->state != BLOCK_STATE__USED || block->size == 0)
		FATAL("block->state %d block->size %zu", block->state, block->size);
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
				ASSERT(map__destroy(map));
		}
	}
	else if (block->type == BLOCK_TYPE__LARGE)
	{
		list_del(&block->list);
		ASSERT(internal_munmap(block, block->size));
	}
	else
		FATAL("block->type %d", block->type);
	return (TRUE);
}
