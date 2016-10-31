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
}		t_block_state;

/*
** block type
*/
typedef enum	e_block_type
{
	BLOCK_TYPE__UNDEFINED = 0,
	BLOCK_TYPE__TINY,
	BLOCK_TYPE__SMALL,
	BLOCK_TYPE__LARGE
}		t_block_type;

/*
** map
*/
typedef struct	s_map_info
{
	size_t		size;
	size_t		map_free_count;
	t_block_type	block_type;
	size_t		total_block_size;
	size_t		user_block_size;
	size_t		block_count_max;
}		t_map_info;

typedef struct	s_map
{
	t_map_info	*const info;

	size_t		block_count;
	t_list		block_list;

	t_list		list;
}		t_map;

t_map	*map__create(const t_map_info *map_info);
bool	map__destroy(t_map *map);

void	*internal_mmap(size_t size);
bool	internal_munmap(void *addr, size_t size);

/*
** block
*/
typedef struct	s_block
{
	t_block_type	type;
	size_t		size;
	t_block_state	state;

	struct s_map	*const map_addr;

	t_list		list;
}		t_block;

t_block		*block_retrieve(t_map_info *map_info, t_list *map_list_head);
bool		  block_return(t_block *block);

# define BLOCK_TO_AREA_OFFSET(block)	((void *)block + sizeof(t_block))
# define AREA_TO_BLOCK_OFFSET(area)	((void *)area - sizeof(t_block))

/*
** context
*/
typedef struct	s_context
{
	bool		is_initialized;

	size_t		total_allocated_memory;

	t_map_info	tiny_info;
	t_list		tiny_list_head;

	t_map_info	small_info;
	t_list		small_list_head;

	t_map_info	large_info;
	t_list		large_list_head;

}		t_context;

bool	get_context(t_context **out_context);

#endif
