#ifndef INTERNAL_MALLOC_H
# define INTERNAL_MALLOC_H

# include "../ft_malloc.h"
# include "list.h"
# include "types.h"
# include "log.h"
# include "libft.h"

/*
** Internal structures and functions
*/

/*
** fifo_type
*/
typedef enum	e_fifo_type
{
	FIFO_TYPE__UNDEFINED = 0,
	FIFO_TYPE__FREE,
	FIFO_TYPE__USED
}				t_fifo_type;

/*
** mem_handle type
*/
typedef enum	e_mem_type
{
	MEM_TYPE__UNDEFINED = 0,
	MEM_TYPE__TINY,
	MEM_TYPE__SMALL,
	MEM_TYPE__LARGE
}				t_mem_type;

/*
** block
*/
typedef struct	s_block
{
	t_mem_type			mem_type;
	t_fifo_type			fifo_type;
	t_list				fifo_list;
	size_t				size;
}				t_block;

# define AREA_TO_BLOCK_OFFSET(area, block)	{block = area - sizeof(t_block);}
# define BLOCK_TO_AREA_OFFSET(block, area)	{area = block; area += sizeof(t_block);}

/*
** mem_handle
*/
typedef struct	s_mem_handle
{
	const t_mem_type	mem_type;
	size_t				block_count;
	size_t				block_size;

	size_t				map_size;
	void				*map;

	t_list				fifo_free;
	t_list				fifo_used;
}				t_mem_handle;

bool	mem_handle__initialize(t_mem_handle	*mem_handle, const size_t map_size);
bool	mem_handle__finalize(t_mem_handle *mem_handle);

/*
** internal_context
*/
typedef struct	s_internal_context
{
	bool			is_initialized;
	t_mem_handle	tiny;
	t_mem_handle	small;
	t_mem_handle	large;
}				t_internal_context;

bool	get_internal_context(t_internal_context **out_context);

#endif
