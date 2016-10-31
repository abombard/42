/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_insert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 14:52:26 by abombard          #+#    #+#             */
/*   Updated: 2016/04/19 14:52:29 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

void	list_insert(t_list *new, t_list *head, unsigned int index)
{
	t_list	*pos;

	pos = list_nth(head, index);
	list_add_tail(new, pos);
}
