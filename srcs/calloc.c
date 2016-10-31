/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 11:43:59 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 12:30:11 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc.h"

void	*calloc(size_t nmemb, size_t size)
{
	size_t	alloc_size;
	void	*alloc;

	alloc_size = nmemb * size;
	if (alloc_size == 0)
		return (NULL);
	alloc = malloc(alloc_size);
	if (alloc == NULL)
		return (NULL);
	ft_bzero(alloc, alloc_size);
	return (alloc);
}
