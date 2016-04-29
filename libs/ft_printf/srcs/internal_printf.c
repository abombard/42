/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_printf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 12:45:46 by abombard          #+#    #+#             */
/*   Updated: 2016/04/19 12:45:47 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uint8_t	switch_case(t_buffer *dst, char c, va_list ap, t_flags *flag)
{
	if (c == 'd' || c == 'i')
		case_nbr(dst, ap, flag);
	else if (c == 'o')
		case_o(dst, ap, flag);
	else if (c == 'u')
		case_u(dst, ap, flag);
	else if (c == 'x')
		case_x(dst, ap, flag);
	else if (c == 'X')
		case_x_maj(dst, ap, flag);
	else if (c == 'p')
		case_p(dst, ap, flag);
	else if (c == 's')
		case_s(dst, ap, flag);
	else if (c == 'c')
		case_c(dst, ap, flag);
	else
		return (0);
	return (1);
}

static uint32_t	get_argument_on_stack(t_buffer *dst, char *fmt, va_list ap)
{
	uint32_t		len;
	t_flags			flag;

	len = 1;
	ft_memset(&flag, 0, sizeof(flag));
	while (fmt[len])
	{
		if (!flag_get_main(fmt, &len, &flag))
			break ;
		len += 1;
	}
	if ('A' <= fmt[len] && fmt[len] <= 'Z'
		&& fmt[len] != 'X')
	{
		flag.l = 1;
		fmt[len] += 40;
	}
	if (!switch_case(dst, fmt[len], ap, &flag))
		return (0);
	flag_apply_main(dst, &flag);
	return (len + 1);
}

static uint32_t	get_next_part(t_buffer *dst, char *fmt, va_list ap)
{
	uint32_t		len;

	if (*fmt == '%')
		len = get_argument_on_stack(dst, fmt, ap);
	else
	{
		len = 0;
		while (fmt[len] && fmt[len] != '%')
		{
			dst->data[len] = fmt[len];
			len++;
		}
		dst->size = len;
	}
	return (len);
}

uint8_t			internal_printf(int fd, char *fmt, va_list ap, uint32_t *ret)
{
	char			str[PRINTF_BUFFER_SIZE_MAX];
	uint32_t		str_index;
	t_buffer		buffer;

	str_index = 0;
	while (*fmt != 0)
	{
		ft_memset(buffer.data, 0, INTERNAL_DATA_SIZE_MAX);
		buffer.size = 0;
		fmt += get_next_part(&buffer, fmt, ap);
		ft_memcpy(str + str_index, buffer.data, buffer.size);
		str_index += buffer.size;
		if (str_index + INTERNAL_DATA_SIZE_MAX >= PRINTF_BUFFER_SIZE_MAX)
		{
			write(fd, str, str_index);
			*ret += str_index;
			str_index = 0;
		}
	}
	if (str_index)
	{
		write(fd, str, str_index);
		*ret += str_index;
	}
	return (1);
}
