/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_move.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:30:14 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 15:30:17 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

void	list_move(t_list *list, t_list *head)
{
	internal_list_del(list->prev, list->next);
	list_add(list, head);
}

void	list_move_tail(t_list *list, t_list *head)
{
	internal_list_del(list->prev, list->next);
	list_add_tail(list, head);
}
