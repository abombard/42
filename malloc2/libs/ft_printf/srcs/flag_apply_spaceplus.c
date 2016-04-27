/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_apply_spaceplus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:43:28 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 13:43:49 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

uint8_t		flag_apply_spaceplus(t_buffer *dst, t_flags *flag)
{
	t_buffer		tmp;
	char			*c;

	c = flag->space && !flag->plus ? " " : "+";
	if (dst->data[0] != '-')
	{
		tmp.size = 1;
		ft_memcpy(tmp.data, c, tmp.size);
		ft_memcpy(tmp.data + tmp.size, dst->data, dst->size);
		dst->size += tmp.size;
		ft_memcpy(dst->data, tmp.data, dst->size);
	}
	return (1);
}
