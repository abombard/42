/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_apply_field.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:32:09 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 13:36:15 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uint8_t	field_bfore(t_buffer *dst, uint32_t len, uint8_t c)
{
	uint32_t		x;

	x = 1;
	dst->size += len;
	if (dst->size > INTERNAL_DATA_SIZE_MAX)
	{
		ft_putendl_fd("error: field_before: dst->size > INTERNAL_DATA_SIZE_MAX",
						2);
		return (0);
	}
	while (x < dst->size)
	{
		dst->data[x++] = c;
	}
	return (1);
}

static uint8_t	field_after(t_buffer *dst, uint32_t len, uint8_t c)
{
	t_buffer		tmp;
	uint32_t		x;

	x = 0;
	while (len)
	{
		tmp.data[x] = c;
		x++;
		len--;
	}
	ft_memcpy(&tmp.data[x], dst->data, dst->size);
	dst->size += x;
	if (dst->size > INTERNAL_DATA_SIZE_MAX)
	{
		ft_putendl_fd("error: field_before: dst->size > INTERNAL_DATA_SIZE_MAX",
						2);
		return (0);
	}
	ft_memcpy(dst->data, tmp.data, dst->size);
	return (1);
}

uint8_t			flag_apply_field(t_buffer *dst, t_flags *flag)
{
	uint32_t		len;
	uint8_t			c;

	if (flag->zero && (!flag->moins || !flag->field))
		c = '0';
	else
		c = ' ';
	len = flag->field - dst->size;
	if (flag->type == INTERNAL_PRINTF_POINTER && c == '0')
		len -= 2;
	else if (flag->space && c == '0')
		len -= 1;
	if (flag->moins && len > 0)
	{
		field_bfore(dst, len, c);
	}
	else if (len > 0)
	{
		field_after(dst, len, c);
	}
	return (1);
}
