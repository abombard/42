#ifndef INTERNAL_MALLOC_H
# define INTERNAL_MALLOC_H

#include "types.h"
#include "log.h"
#include "libft.h"
#include "list.h"

/*
** block state
*/
typedef enum	e_block_state
{
	BLOCK_STATE__UNDEFINED = 0,
	BLOCK_STATE__FREE,
	BLOCK_STATE__USED
}				t_block_state;

/*
** block type
*/
typedef enum	e_block_type
{
	BLOCK_TYPE__UNDEFINED = 0,
	BLOCK_TYPE__TINY,
	BLOCK_TYPE__SMALL,
	BLOCK_TYPE__LARGE
}				t_block_type;

/*
** block
*/
typedef struct	s_block
{
	t_list			list;

	t_block_type    type;
	size_t		    size;
	t_block_state	state;

	struct s_map	*map_addr;
}				t_block;

/*
** map
*/
typedef struct	s_map
{
	size_t		    size;

    t_block_type	block_type;

	size_t			block_count_max;
	size_t			block_count;
	size_t			block_size;
	t_list			block_list;

	t_block			*first_block_free;

	t_list			list;
}				t_map;

/*
** context
*/
typedef struct  s_context
{
	bool	is_initialized;

	size_t	tiny_block_size;
    t_list	tiny;

	size_t	small_block_size;
	t_list	small;

	t_list	large;
}				t_context;

#endif
