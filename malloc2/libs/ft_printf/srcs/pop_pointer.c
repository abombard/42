/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_pointer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 14:03:59 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 14:06:47 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uint8_t		get_nbr_hex(t_buffer *dst, unsigned long long nbr)
{
	char				*base;
	char				tmp[14];
	int					x;

	base = "0123456789abcdef";
	dst->size = 12;
	tmp[13] = 0;
	x = 12;
	while (nbr > 0)
	{
		tmp[x--] = base[nbr % ft_strlen(base)];
		nbr /= ft_strlen(base);
	}
	ft_strcpy((char *)dst->data, tmp + x + 1);
	dst->size = ft_strlen(tmp) - 1;
	return (1);
}

uint8_t				case_p(t_buffer *dst, va_list ap, t_flags *flag)
{
	unsigned long				pt;

	pt = va_arg(ap, unsigned long);
	if (pt == 0)
	{
		dst->size = sizeof("(nil)");
		ft_memcpy(dst->data, "(nil)", dst->size);
	}
	get_nbr_hex(dst, pt);
	flag->type = INTERNAL_PRINTF_POINTER;
	return (1);
}
