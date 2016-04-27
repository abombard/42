/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_apply_diez.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:29:53 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 13:31:51 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

uint8_t		flag_apply_diez(t_buffer *dst, uint32_t type)
{
	t_buffer		tmp;

	if (!ft_memcmp(dst->data, "(nil)", 4) || !ft_memcmp(dst->data, "(Null)", 6)
		|| !ft_memcmp(dst->data, "0", 1))
		return (0);
	if (type == INTERNAL_PRINTF_UOCTAL)
	{
		tmp.size = 1;
		ft_memcpy(tmp.data, "0", tmp.size);
	}
	else if (type == INTERNAL_PRINTF_UHEXA_MIN ||
			type == INTERNAL_PRINTF_POINTER)
	{
		tmp.size = 2;
		ft_memcpy(tmp.data, "0x", tmp.size);
	}
	else if (type == INTERNAL_PRINTF_UHEXA_MAJ)
	{
		tmp.size = 2;
		ft_memcpy(tmp.data, "0X", tmp.size);
	}
	ft_memcpy(tmp.data + tmp.size, dst->data, dst->size);
	dst->size += tmp.size;
	ft_memcpy(dst->data, tmp.data, dst->size);
	return (1);
}
