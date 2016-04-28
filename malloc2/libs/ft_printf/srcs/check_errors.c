/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:23:47 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 14:06:22 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uint32_t	count_char(const char *s, char c)
{
	uint32_t		ret;
	uint32_t		i;

	ret = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			ret += 1;
		i += 1;
	}
	return (ret);
}

uint8_t			check_errors(const char *fmt)
{
	int	approx_args_size;

	approx_args_size = (count_char(fmt, '%') * INTERNAL_DATA_SIZE_MAX);
	if (fmt == NULL)
		ft_putendl_fd("fmt NULL", 2);
	if (ft_strlen(fmt) + approx_args_size > PRINTF_BUFFER_SIZE_MAX)
	{
		ft_putendl_fd("error: printf_basic: \
You might need to update DATA_SIZE_MAX or BUFFER_SIZE_MAX", 2);
		return (0);
	}
	return (1);
}
