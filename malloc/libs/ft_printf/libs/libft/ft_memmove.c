/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 17:58:16 by abombard          #+#    #+#             */
/*   Updated: 2014/11/21 09:15:11 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*dup;

	if (!dst || !src || !len)
		return (NULL);
	dup = ft_strdup((char*)src);
	dst = (void*)ft_strncpy((char*)dst, dup, len);
	free(dup);
	return (dst);
}
