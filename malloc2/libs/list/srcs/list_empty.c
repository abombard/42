/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_empty.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:35:55 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 15:35:57 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

int		list_empty(t_list *head)
{
	return (head->next == head);
}
