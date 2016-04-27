/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_nth.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 14:53:02 by abombard          #+#    #+#             */
/*   Updated: 2016/04/19 14:53:03 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

t_list		*list_nth(const t_list *head, const int index)
{
	t_list		*tmp;
	int			i;

	tmp = (t_list *)head;
	i = 0;
	if (index > 0)
	{
		while (i < index)
		{
			tmp = tmp->next;
			i++;
		}
	}
	else if (index < 0)
	{
		while (i > index)
		{
			tmp = tmp->prev;
			i--;
		}
	}
	return (tmp);
}
