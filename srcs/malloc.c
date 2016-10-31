/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 12:32:42 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 13:47:57 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc.h"

static t_block	*get_preallocated_block(t_context *context, size_t size)
{
	t_list			*map_list_head;
	t_map_info		*map_info;
	t_block			*block;

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
		FATAL("block_retrieve failed %s", "");
	return (block);
}

void			*malloc(size_t size)
{
	t_context		*context;
	t_block			*block;
	void			*area;

	fprintf(stderr, "malloc size %zu\n", size);
	if (size == 0)
		return (NULL);
	ASSERT(get_context(&context));
	if (size < context->small_info.user_block_size)
	{
		block = get_preallocated_block(context, size);
		if (block == NULL)
			FATAL("get_preallocated_block failed size %zu", size);
	}
	else
	{
		block = internal_mmap(sizeof(t_block) + size);
		if (block == NULL)
			FATAL("internal_mmap() failed block_size %zu", size);
		list_add_tail(&block->list, &context->large_list_head);
		block->type = BLOCK_TYPE__LARGE;
	}
	block->state = BLOCK_STATE__USED;
	block->size = size;
	area = BLOCK_TO_AREA_OFFSET(block);
	return (area);
}
