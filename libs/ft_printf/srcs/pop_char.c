/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_char.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 12:53:37 by abombard          #+#    #+#             */
/*   Updated: 2016/04/19 12:57:12 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <wchar.h>

static void		case_s_wchar(t_buffer *dst, va_list ap)
{
	uint32_t	*wc;
	int			x;

	wc = (unsigned int *)va_arg(ap, wchar_t *);
	x = 0;
	while (wc[x])
	{
		get_char(dst, wc[x]);
		x += 1;
	}
}

static void		internal_case_s(t_buffer *dst, va_list ap)
{
	char	*s;
	size_t	len;

	s = va_arg(ap, char *);
	len = ft_strlen(s);
	if (len + 1 > INTERNAL_DATA_SIZE_MAX)
	{
		ft_memcpy(dst->data, s, INTERNAL_DATA_SIZE_MAX - 2);
		dst->data[INTERNAL_DATA_SIZE_MAX - 2] = '$';
		dst->data[INTERNAL_DATA_SIZE_MAX - 1] = '\0';
		dst->size = INTERNAL_DATA_SIZE_MAX;
	}
	else
	{
		ft_memcpy(dst->data, s, len + 1);
		dst->size = len + 1;
	}
}

uint8_t			case_s(t_buffer *dst, va_list ap, t_flags *flag)
{
	if (flag->l)
		case_s_wchar(dst, ap);
	else
	{
		internal_case_s(dst, ap);
	}
	if (dst->size == 0)
	{
		dst->size = sizeof("(null)") - 1;
		ft_memcpy(dst->data, "(null)", dst->size);
	}
	flag->type = INTERNAL_PRINTF_STRING;
	return (1);
}

uint8_t			case_c(t_buffer *dst, va_list ap, t_flags *flag)
{
	unsigned int	ch;

	ch = va_arg(ap, unsigned int);
	get_char(dst, ch);
	flag->type = INTERNAL_PRINTF_CHAR;
	return (1);
}
