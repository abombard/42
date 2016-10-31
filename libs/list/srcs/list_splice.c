/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_splice.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:41:57 by abombard          #+#    #+#             */
/*   Updated: 2016/04/01 18:39:29 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

void	internal_list_splice(t_list *list, t_list *head)
{
	t_list	*first;
	t_list	*last;
	t_list	*at;

	first = list->next;
	last = list->prev;
	at = head->next;
	first->prev = head;
	head->next = first;
	last->next = at;
	at->prev = last;
}

void	list_splice(t_list *list, t_list *head)
{
	if (!list_empty(list))
	{
		internal_list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}
