/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 13:08:11 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 13:09:47 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc.h"

/*
** map create
*/

bool	internal_map__init(const t_map_info *map_info, t_map *const map)
{
	t_block	*block;
	size_t	block_offset;

	if (!map_info || !map)
	{
		LOG_ERROR("map_info %p map %p", (void *)map_info, (void *)map);
		return (FALSE);
	}
	INIT_LIST_HEAD(&map->block_list);
	block_offset = sizeof(t_map);
	while (block_offset < map_info->size)
	{
		block = (void *)map + block_offset;
		ft_memcpy((void *)&block->type,
				(void *)&map_info->block_type,
				sizeof(t_block_type));
		block->size = 0;
		block->state = BLOCK_STATE__FREE;
		ft_memcpy((void *)&block->map_addr, (void *)&map, sizeof(t_map *));
		list_add_tail(&block->list, &map->block_list);
		block_offset += map_info->total_block_size;
	}
	return (TRUE);
}

t_map	*map__create(const t_map_info *map_info)
{
	t_map	*map;

	map = internal_mmap(sizeof(t_block) + map_info->size);
	if (map == NULL)
	{
		LOG_ERROR("internal_mmap() failed map_size %zu", map_info->size);
		return (NULL);
	}
	ft_memcpy((void *)&map->info, (void *)&map_info, sizeof(t_map_info *));
	map->block_count = 0;
	if (!internal_map__init(map_info, map))
	{
		LOG_ERROR("internal_map__init() failed %s", "");
		return (NULL);
	}
	return (map);
}

/*
** map destroy
*/

bool	map__destroy(t_map *map)
{
	list_del(&map->list);
	if (!internal_munmap(map, map->info->size))
	{
		LOG_ERROR("internal_munmap() failed %s", "");
		return (FALSE);
	}
	return (TRUE);
}
