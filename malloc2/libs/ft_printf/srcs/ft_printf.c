/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:44:43 by abombard          #+#    #+#             */
/*   Updated: 2016/04/01 17:57:55 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *fmt, ...)
{
	uint32_t	ret;
	va_list		ap;
	char		*fmt_p;

	if (!check_errors(fmt))
	{
		return (0);
	}
	va_start(ap, fmt);
	fmt_p = (char *)fmt;
	ret = 0;
	internal_printf(1, fmt_p, ap, &ret);
	return (ret);
}
