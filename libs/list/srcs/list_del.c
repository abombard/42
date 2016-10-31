/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_del.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:30:00 by abombard          #+#    #+#             */
/*   Updated: 2016/04/01 18:45:20 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

void	internal_list_del(t_list *prev, t_list *next)
{
	prev->next = next;
	next->prev = prev;
}

void	list_del(t_list *entry)
{
	internal_list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}
