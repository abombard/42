/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_apply_precision.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:42:18 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 13:43:17 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

uint8_t		flag_apply_precision(t_buffer *dst, t_flags *flag)
{
	t_buffer		tmp;
	int				len;

	if (flag->type != INTERNAL_PRINTF_CHAR)
	{
		len = flag->precision - dst->size;
		if (len > 0)
		{
			tmp.size = flag->precision;
			ft_memset(tmp.data, '0', len);
			ft_memcpy(tmp.data + len, dst->data, dst->size);
			dst->size += len;
			ft_memcpy(dst->data, tmp.data, dst->size);
		}
		else
			dst->size += len;
	}
	return (1);
}
