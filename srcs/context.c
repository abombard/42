/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 12:47:25 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 13:07:57 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc.h"

#define BLOCK_COUNT_MIN	100

static size_t	compute_total_block_size(const size_t map_size)
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

static void		init_block_info(t_block_type type,
		size_t size,
		t_map_info *info)
{
	info->map_free_count = 0;
	info->block_type = type;
	info->size = size;
	info->total_block_size = compute_total_block_size(size - sizeof(t_map));
	info->user_block_size = info->total_block_size - sizeof(t_block);
	info->block_count_max = size / info->total_block_size;
}

static bool		init(t_context *context)
{
	context->total_allocated_memory = 0;
	init_block_info(BLOCK_TYPE__TINY, TINY_MAP_SIZE, &context->tiny_info);
	init_block_info(BLOCK_TYPE__SMALL, SMALL_MAP_SIZE, &context->small_info);
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

extern bool		get_context(t_context **out_context)
{
	static t_context	context = { .is_initialized = FALSE };

	*out_context = NULL;
	if (context.is_initialized == FALSE)
	{
		if (!init(&context))
			return (FALSE);
		context.is_initialized = TRUE;
	}
	*out_context = &context;
	return (TRUE);
}
