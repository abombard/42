/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_octal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 14:03:16 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 14:03:25 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uint8_t	get_nbr_oct(t_buffer *dst, unsigned long nbr)
{
	char	*base;
	char	tmp[36];
	int		x;

	if (nbr == 0)
	{
		dst->size = 0;
		return (1);
	}
	base = "01234567";
	x = 35;
	tmp[x--] = 0;
	while (nbr > 0)
	{
		tmp[x--] = base[nbr % ft_strlen(base)];
		nbr /= ft_strlen(base);
	}
	ft_strcpy((char *)dst->data, tmp + x + 1);
	dst->size = ft_strlen((char *)dst->data);
	return (1);
}

uint8_t			case_o(t_buffer *dst, va_list ap, t_flags *flag)
{
	unsigned long long		n;

	if (flag->h)
		n = (unsigned short)va_arg(ap, int);
	else if (flag->hh)
		n = (unsigned char)va_arg(ap, int);
	else if (flag->l)
		n = (unsigned long)va_arg(ap, unsigned long);
	else if (flag->ll)
		n = va_arg(ap, unsigned long long);
	else
		n = (unsigned int)va_arg(ap, int);
	get_nbr_oct(dst, n);
	flag->type = INTERNAL_PRINTF_UOCTAL;
	return (1);
}
