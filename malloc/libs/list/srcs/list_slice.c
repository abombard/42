/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_slice.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 14:52:43 by abombard          #+#    #+#             */
/*   Updated: 2016/04/19 14:52:44 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

t_list		*list_slice(t_list *new,
							t_list *head,
							const int index,
							int len)
{
	t_list		*start;
	t_list		*end;
	t_list		*prev;
	t_list		*next;

	if (!new || !head || !len)
		return ((t_list *)0);
	INIT_LIST_HEAD(new);
	start = list_nth(head, index);
	end = list_nth(head, index + len - 1);
	prev = start->prev;
	next = end->next;
	prev->next = next;
	next->prev = prev;
	new->next = start;
	new->prev = end;
	start->prev = new;
	end->next = new;
	return (new);
}
