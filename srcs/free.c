/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 11:43:34 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 13:47:29 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc.h"

void	free(void *ptr)
{
	t_block	*block;

	fprintf(stderr, "free\n");
	if (ptr == NULL)
		return ;
	block = AREA_TO_BLOCK_OFFSET(ptr);
	if (!block_return(block))
		LOG_ERROR("block_return() failed %s", "");
}
