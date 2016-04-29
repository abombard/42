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

	t_block_type	type;
	size_t			size;
	t_block_state	state;

	struct s_map	*map_addr;
}				t_block;

# define BLOCK_TO_AREA_OFFSET(block)	((void *)block + sizeof(t_block))
# define AREA_TO_BLOCK_OFFSET(area)		((void *)area - sizeof(t_block))

/*
** map
*/
typedef struct	s_map
{
	size_t			size;

	t_block_type	block_type;

	size_t			block_count_max;
	size_t			block_count;
	size_t			block_size;
	t_list			block_list;

	t_block			*first_block_free;

	t_list			list;
}				t_map;

typedef struct	s_map_handle
{
	size_t	block_type;
	size_t	map_size;
	size_t	block_size;
	t_list	list_head;
}				t_map_handle;

t_map	*map__create(const size_t map_size,
					 const t_block_type block_type,
					 const size_t block_size);
bool	map__destroy(t_map *map);

/*
** context
*/
typedef struct	s_context
{
	bool			is_initialized;

	t_map_handle	tiny;

	t_map_handle	small;

	t_list		large;
}				t_context;

#endif
