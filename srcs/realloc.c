/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 12:41:48 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 12:41:58 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc.h"

void	*realloc(void *ptr, size_t size)
{
	t_block	*ptr_block;
	t_block	*new_block;
	void	*new_area;

	if (ptr == NULL)
		return (NULL);
	ptr_block = AREA_TO_BLOCK_OFFSET(ptr);
	if (size < ptr_block->size)
	{
		free(ptr);
		return (NULL);
	}
	new_area = malloc(size);
	if (new_area == NULL)
	{
		free(ptr);
		return (NULL);
	}
	new_block = AREA_TO_BLOCK_OFFSET(new_area);
	ft_memcpy(new_area, ptr, ptr_block->size);
	free(ptr);
	return (new_area);
}
