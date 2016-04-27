/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_apply_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:36:35 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 13:41:55 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uint8_t	flag_apply_order_1(t_buffer *dst, t_flags *flag)
{
	if (flag->field)
	{
		flag_apply_field(dst, flag);
	}
	if ((flag->plus || flag->space) &&
		(flag->type == INTERNAL_PRINTF_INT ||
		flag->type == INTERNAL_PRINTF_POINTER))
	{
		flag_apply_spaceplus(dst, flag);
	}
	if (flag->precision)
	{
		flag_apply_precision(dst, flag);
	}
	if (flag->diez || flag->type == INTERNAL_PRINTF_POINTER)
	{
		flag_apply_diez(dst, flag->type);
	}
	return (1);
}

static uint8_t	flag_apply_order_2(t_buffer *dst, t_flags *flag)
{
	if (flag->precision)
	{
		flag_apply_precision(dst, flag);
	}
	if (flag->diez || flag->type == INTERNAL_PRINTF_POINTER)
	{
		flag_apply_diez(dst, flag->type);
	}
	if ((flag->plus || flag->space) &&
		(flag->type == INTERNAL_PRINTF_INT ||
		flag->type == INTERNAL_PRINTF_POINTER))
	{
		flag_apply_spaceplus(dst, flag);
	}
	if (flag->field)
	{
		flag_apply_field(dst, flag);
	}
	return (1);
}

uint8_t			flag_apply_main(t_buffer *dst, t_flags *flag)
{
	if (flag->zero && (!flag->moins || !flag->field))
	{
		flag_apply_order_1(dst, flag);
	}
	else
	{
		flag_apply_order_2(dst, flag);
	}
	return (1);
}
