/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 17:15:38 by abombard          #+#    #+#             */
/*   Updated: 2016/03/30 17:15:41 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_atoi(const char *str)
{
	long long	nbr;
	int			x;

	nbr = 0;
	x = 1;
	while (*str)
	{
		if ((*str == '-' || *str == '+') && ft_isdigit(*(str + 1)) == 1)
			x = (*(str++) == '-' ? -1 : 1);
		if (ft_isdigit(*str) == 1)
		{
			while (ft_isdigit(*str) == 1)
			{
				nbr = (nbr * 10) + (*str - '0');
				str++;
			}
			return ((int)(nbr * x));
		}
		if (ft_isspace(*str))
			str++;
		else
			return (0);
	}
	return (0);
}

long long	ft_atol(const char *str)
{
	long long	nbr;
	int			x;

	nbr = 0;
	x = 1;
	while (*str)
	{
		if ((*str == '-' || *str == '+') && ft_isdigit(*(str + 1)) == 1)
			x = (*(str++) == '-' ? -1 : 1);
		if (ft_isdigit(*str) == 1)
		{
			while (ft_isdigit(*str) == 1)
			{
				nbr = (nbr * 10) + (*str - '0');
				str++;
			}
			return ((nbr * x));
		}
		if (ft_isspace(*str))
			str++;
		else
			return (0);
	}
	return (0);
}
