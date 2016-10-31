/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/31 11:14:18 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 12:46:39 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>

void		fee(void *pt);
void		*malloc(size_t size);
void		*ealloc(void *pt, size_t size);
void		show_memory(void);
void		*calloc(size_t nmemb, size_t size);

#endif
