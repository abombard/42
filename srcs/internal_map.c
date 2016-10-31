/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 12:30:37 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 12:32:34 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_malloc.h"
#include <sys/mman.h>
#include <sys/resource.h>

/*
** mmap
*/

static size_t	real_map_size(size_t size)
{
	return (size + size % getpagesize());
}

extern void		*internal_mmap(size_t size)
{
	t_context		*context;
	struct rlimit	rlim;
	void			*map;

	if (!get_context(&context))
		return (NULL);
	context->total_allocated_memory += real_map_size(size);
	if (getrlimit(RLIMIT_AS, &rlim) != 0)
	{
		LOG_ERROR("getrlimit() failed %s", "");
		return (NULL);
	}
	if (context->total_allocated_memory > rlim.rlim_max)
	{
		LOG_ERROR("Reached max allocated memory %zu", (size_t)rlim.rlim_max);
		return (NULL);
	}
	map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (map == MAP_FAILED)
	{
		LOG_ERROR("mmap() failed size %zu", size);
		return (NULL);
	}
	return (map);
}

/*
** munmap
*/

extern bool		internal_munmap(void *addr, size_t size)
{
	t_context	*context;

	if (!get_context(&context))
		return (FALSE);
	context->total_allocated_memory -= real_map_size(size);
	if (munmap(addr, size) != 0)
	{
		LOG_ERROR("munmap() failed addr %p size %zu", (void *)addr, size);
		return (FALSE);
	}
	return (TRUE);
}
