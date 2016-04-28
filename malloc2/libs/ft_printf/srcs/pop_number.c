/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_number.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 14:01:40 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 14:50:08 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uint8_t		make_nbr(uint8_t *nbr, long long n, int len)
{
	unsigned long long	i;

	i = n;
	nbr[0] = '0';
	if (n < 0)
	{
		nbr[0] = '-';
		i = -n;
	}
	while (i > 0)
	{
		nbr[len - 1] = i % 10 + '0';
		i = i / 10;
		len--;
	}
	return (1);
}

static uint8_t		ft_itoa_lg(t_buffer *dst, long long n)
{
	unsigned long long	old_n;

	dst->size = 0;
	old_n = n;
	if (n < 0)
	{
		dst->size++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		dst->size++;
	}
	make_nbr(dst->data, old_n, dst->size);
	return (1);
}

uint8_t				case_nbr(t_buffer *dst, va_list ap, t_flags *flag)
{
	long long		n;

	if (flag->j)
		n = (intmax_t)va_arg(ap, int);
	else if (flag->z)
		n = (size_t)va_arg(ap, int);
	else if (flag->h)
		n = (short)va_arg(ap, int);
	else if (flag->hh)
		n = (char)va_arg(ap, int);
	else if (flag->l)
		n = (long)va_arg(ap, long);
	else if (flag->ll)
		n = va_arg(ap, long long);
	else
		n = (int)va_arg(ap, int);
	ft_itoa_lg(dst, n);
	dst->size = ft_strlen((char *)dst->data);
	flag->type = INTERNAL_PRINTF_INT;
	return (1);
}
