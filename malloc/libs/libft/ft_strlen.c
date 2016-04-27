/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/03 13:01:51 by abombard          #+#    #+#             */
/*   Updated: 2015/06/10 11:49:28 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strlen(const char *str)
{
	int		x;

	if (!str)
		return (-1);
	x = 0;
	while (str[x])
		x++;
	return (x);
}
