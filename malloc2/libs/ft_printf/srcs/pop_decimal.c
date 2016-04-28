/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_decimal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:57:15 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 13:57:54 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uint8_t	ft_itoa_ull(t_buffer *dst, unsigned long long n)
{
	unsigned long long	old_n;
	uint32_t			len;

	len = 0;
	old_n = n;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	dst->size = len;
	n = old_n;
	while (n > 0)
	{
		dst->data[len - 1] = n % 10 + '0';
		n = n / 10;
		len--;
	}
	return (1);
}

uint8_t			case_u(t_buffer *dst, va_list ap, t_flags *flag)
{
	unsigned long long	n;

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
	ft_itoa_ull(dst, n);
	flag->type = INTERNAL_PRINTF_UDECIMAL;
	return (1);
}
