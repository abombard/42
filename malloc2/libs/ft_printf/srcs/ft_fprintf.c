/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 13:44:20 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 14:52:30 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_fprintf(int fd, const char *fmt, ...)
{
	uint32_t		ret;
	va_list			ap;
	char			*fmt_p;

	if (!check_errors(fmt))
		return (0);
	va_start(ap, fmt);
	fmt_p = (char *)fmt;
	ret = 0;
	internal_printf(fd, fmt_p, ap, &ret);
	return (1);
}
