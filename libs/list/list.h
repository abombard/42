/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 14:49:30 by abombard          #+#    #+#             */
/*   Updated: 2016/10/31 15:16:09 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

/*
** Define the offset of the member m of type t.
** @param t: type		the type of the structure
** @param m: member		the member to get the offset
*/

# undef OFFSETOF
# define OFFSETOF(type, member) ((size_t) &((type *)0)->member)

# define OFST_OF(t, m) OFFSETOF(t, m)

/*
** Cast a member of a structure of type t pointed by ptr and named m out to the
** containing structure.
** @param p: ptr		the pointer to the member
** @param t: type		the type of the container structure
** @param m: member		the name of the member
*/

# undef TYPE_M
# define TYPE_M(t,m) const typeof(((t*)0)->m)

# undef CONTAINER_OF
# define CONTAINER_OF(p,t,m)({TYPE_M(t,m)*mp=(p);(t*)((char*)mp-OFST_OF(t,m));})

/*
** Double linked list implementation
*/
typedef struct	s_list
{
	struct s_list	*prev;
	struct s_list	*next;
}				t_list;

/*
** Initialize list
*/
# define LIST_HEAD_INIT(name) { &(name), &(name) }

# define LIST_HEAD(name) t_list	name = LIST_HEAD_INIT(name)

# define INIT_LIST_HEAD(ptr) (ptr)->next = (ptr); (ptr)->prev = (ptr);

/*
** list add: add an element to the list
*/
extern void		list_add (t_list *new, t_list *head);
extern void		list_add_tail (t_list *new, t_list *head);

/*
** list del: delete an element from the list
*/
extern void		list_del (t_list *entry);

/*
** list move: move an element from one list to another
*/
extern void		list_move (t_list *list, t_list *head);
extern void		list_move_tail (t_list *list, t_list *head);

/*
** check if list is empty
*/
extern int		list_empty (t_list *head);

/*
** list len: return the length of the list
*/
extern int		list_len (t_list *head);

/*
** list splice: concatenate two lists
*/
extern void		list_splice (t_list *list, t_list *head);

/*
** list nth: return the nth element of a list
*/
extern t_list	*list_nth (const t_list *head, const int index);

/*
** list slice: get a part of a list
*/
extern t_list	*list_slice(t_list		*new,
							t_list		*head,
							const int	index,
							int			len);

/*
** list insert
*/
extern void		list_insert(t_list *new, t_list *head, unsigned int index);

/*
** list foreach: iterate through element of a list
*/
# define LIST_FOREACH(h, p) p=(t_list*)h;while((p=p->next)&&p!=h)

# define LIST_FOREACH_PREV(head,pos) pos=head;while((pos=pos->prev)&&pos!=head)

/*
**# define LIST_FOREACH_SAFE(head, pos, save) save=head->next;\
**		   while((pos=save)&&pos!=head&&(save=save->next))
*/

#endif
