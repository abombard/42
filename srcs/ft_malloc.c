#include "internal_malloc.h"
#include <sys/mman.h>

/*
 ** context
 */
#define BLOCK_COUNT_MIN	100
size_t	context__compute_total_block_size(const size_t map_size)
{
	uint32_t	total_block_size;

	total_block_size = map_size / BLOCK_COUNT_MIN;
	total_block_size |= (total_block_size >> 1);
	total_block_size |= (total_block_size >> 2);
	total_block_size |= (total_block_size >> 4);
	total_block_size |= (total_block_size >> 8);
	total_block_size |= (total_block_size >> 16);
	total_block_size -= (total_block_size >> 1);
	return (total_block_size);
}

#define TINY_MAP_SIZE		(getpagesize() * 2)
#define SMALL_MAP_SIZE		(getpagesize() * 10)
static bool	context__init(t_context *context)
{
	context->total_allocated_memory = 0;
	context->tiny_info.map_free_count = 0;
	context->tiny_info.size = TINY_MAP_SIZE;
	context->tiny_info.block_type = BLOCK_TYPE__TINY;
	context->tiny_info.total_block_size = context__compute_total_block_size(TINY_MAP_SIZE - sizeof(t_map));
	context->tiny_info.user_block_size = context->tiny_info.total_block_size - sizeof(t_block);
	context->tiny_info.block_count_max = TINY_MAP_SIZE / (context->tiny_info.total_block_size);
	context->small_info.map_free_count = 0;
	context->small_info.size = SMALL_MAP_SIZE;
	context->small_info.block_type = BLOCK_TYPE__SMALL;
	context->small_info.total_block_size = context__compute_total_block_size(SMALL_MAP_SIZE - sizeof(t_map));
	context->small_info.user_block_size = context->small_info.total_block_size - sizeof(t_block);
	context->small_info.block_count_max = SMALL_MAP_SIZE / (context->small_info.total_block_size);
	context->large_info.map_free_count = 0;
	context->large_info.size = 0;
	context->large_info.block_type = BLOCK_TYPE__LARGE;
	context->large_info.total_block_size = 0;
	context->large_info.user_block_size = 0;
	context->large_info.block_count_max = 0;

	INIT_LIST_HEAD(&context->tiny_list_head);
	INIT_LIST_HEAD(&context->small_list_head);
	INIT_LIST_HEAD(&context->large_list_head);
	return (TRUE);
}

bool	get_context(t_context **out_context)
{
	static t_context	context = {
		.is_initialized = FALSE
	};

	*out_context = NULL;
	if (context.is_initialized == FALSE)
	{
		if (!context__init(&context))
			return (FALSE);
		context.is_initialized = TRUE;
	}
	*out_context = &context;
	return (TRUE);
}

/*
** mmap() / munmap()
*/
static size_t	real_map_size(size_t size)
{
	return (size + size % getpagesize());
}

#include <sys/resource.h>
void	*internal_mmap(size_t size)
{
	t_context	*context;
	struct rlimit	rlim;
	void		*map;

	if (!get_context(&context))
		return (NULL);
	context->total_allocated_memory += real_map_size(size);
	if (getrlimit(RLIMIT_AS, &rlim) != 0)
	{
		LOG_ERROR("getrlimit() failed %s", "");
		return (NULL);
	}
	if (context->total_allocated_memory > rlim.rlim_max)
	{
		LOG_ERROR("Reached max allocated memory %ld", rlim.rlim_max);
		return (NULL);
	}
	map = mmap(0,
			size,
			PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE,
			-1, 0);
	if (map == MAP_FAILED)
	{
		LOG_ERROR("mmap() failed size %zu", size);
		return (NULL);
	}
	return (map);
}

bool	internal_munmap(void *addr, size_t size)
{
	t_context	*context;

	if (!get_context(&context))
		return (FALSE);
	context->total_allocated_memory -= real_map_size(size);
	if (munmap(addr, size) != 0)
	{
		LOG_ERROR("munmap() failed addr %p size %zu", (void *)addr, size);
		return (FALSE);
	}
	return (TRUE);
}

/*
 ** block retrieve() / return()
 */
t_block		*block_retrieve(t_map_info *map_info, t_list *map_list_head)
{
	t_list	*pos;
	t_map	*map;
	t_block	*block;

	LIST_FOREACH(map_list_head, pos)
	{
		map = CONTAINER_OF(pos, t_map, list);
		if (map->block_count < map->info->block_count_max)
			break ;
	}
	if (map_list_head == pos)
	{
		map = map__create(map_info);
		if (map == NULL)
		{
			LOG_ERROR("map__create() failed %s", "");
			return (NULL);
		}
		list_add_tail(&map->list, map_list_head);
	}
	LIST_FOREACH(&map->block_list, pos)
	{
		block = CONTAINER_OF(pos, t_block, list);
		if (block->state == BLOCK_STATE__FREE)
			break ;
	}
	if (&map->block_list == pos)
	{
		LOG_ERROR("Found no free block %s", "");
		return (NULL);
	}
	map->block_count++;
	return (block);
}

bool		block_return(t_block *block)
{
	t_map	*map;

	if (block->state != BLOCK_STATE__USED || block->size == 0 ||
		(block->type != BLOCK_TYPE__TINY && block->type != BLOCK_TYPE__SMALL &&
		block->type != BLOCK_TYPE__LARGE))
	{
		LOG_ERROR("block->state %d block->size %zu block->type %d",
				block->state, block->size, block->type);
		return (FALSE);
	}
	if (block->type == BLOCK_TYPE__TINY || block->type == BLOCK_TYPE__SMALL)
	{
		block->state = BLOCK_STATE__FREE;
		block->size = 0;
		map = block->map_addr;
		map->block_count--;
		if (map->block_count == 0)
		{
			map->info->map_free_count++;
			if (map->info->map_free_count > 1)
			{
				if (!map__destroy(map))
				{
					LOG_ERROR("map__destroy() failed %s", "");
					return (FALSE);
				}
			}
		}
	}
	else if (block->type == BLOCK_TYPE__LARGE)
	{
		list_del(&block->list);
		if (!internal_munmap(block, block->size))
			return (FALSE);
	}
	return (TRUE);
}

/*
 ** map create() / destroy()
 */
bool	internal_map__init(const t_map_info *map_info, t_map *const map)
{
	t_block	*block;
	size_t	block_offset;

	if (!map_info || !map)
	{
		LOG_ERROR("map_info %p map %p", (void *)map_info, (void *)map);
		return (FALSE);
	}
	INIT_LIST_HEAD(&map->block_list);
	block_offset = sizeof(t_map);
	while (block_offset < map_info->size)
	{
		block = (void *)map + block_offset;
		ft_memcpy((void *)&block->type, (void *)&map_info->block_type, sizeof(t_block_type));
		block->size = 0;
		block->state = BLOCK_STATE__FREE;
		ft_memcpy((void *)&block->map_addr, (void *)&map, sizeof(t_map *));
		list_add_tail(&block->list, &map->block_list);
		block_offset += map_info->total_block_size;
	}
	return (TRUE);
}

t_map	*map__create(const t_map_info *map_info)
{
	t_map	*map;

	map = internal_mmap(sizeof(t_block) + map_info->size);
	if (map == NULL)
	{
		LOG_ERROR("internal_mmap() failed map_size %zu", map_info->size);
		return (NULL);
	}
	ft_memcpy((void *)&map->info, (void *)&map_info, sizeof(t_map_info *));
	map->block_count = 0;
	if (!internal_map__init(map_info, map))
	{
		LOG_ERROR("internal_map__init() failed %s", "");
		return (NULL);
	}
	return (map);
}

bool	map__destroy(t_map *map)
{
	list_del(&map->list);
	if (!internal_munmap(map, map->info->size))
	{
		LOG_ERROR("internal_munmap() failed %s", "");
		return (FALSE);
	}
	return (TRUE);
}

/*
 ** show_mem()
 */
#include "ft_printf.h"
void	show_block_memory(char *block_type, void *addr, t_list *head)
{
	t_block	*block;
	void	*area;
	t_list	*pos_block;

	ft_printf("%s : 0x%.6X\n", block_type, (unsigned int)addr);
	LIST_FOREACH(head, pos_block)
	{
		block = CONTAINER_OF(pos_block, t_block, list);
		if (block->state == BLOCK_STATE__USED)
		{
			area = BLOCK_TO_AREA_OFFSET(block);
			ft_printf("0x%.6X - 0x%.6X : %zu octets\n", (unsigned int)area, (unsigned int)(area + block->size), block->size);
		}
	}
}

void	show_large_block_memory(t_list *head)
{
	t_block	*block;
	void	*area;
	t_list	*pos_block;

	LIST_FOREACH(head, pos_block)
	{
		block = CONTAINER_OF(pos_block, t_block, list);
		if (block->state == BLOCK_STATE__USED)
		{
			ft_printf("%s: 0x%.6X\n", "LARGE", (unsigned int)block);
			area = BLOCK_TO_AREA_OFFSET(block);
			ft_printf("0x%.6X - 0x%.6X : %zu octets\n", (unsigned int)area, (unsigned int)(area + block->size), block->size);
		}
	}
}

void	show_map_memory(const t_block_type block_type, t_list *head)
{
	t_map	*map;
	t_list	*pos_map;
	char	*type;

	if (block_type == BLOCK_TYPE__TINY || block_type == BLOCK_TYPE__SMALL)
	{
		if (block_type == BLOCK_TYPE__TINY)
			type = "TINY";
		else if (block_type == BLOCK_TYPE__SMALL)
			type = "SMALL";
		LIST_FOREACH(head, pos_map)
		{
			map = CONTAINER_OF(pos_map, t_map, list);
			show_block_memory(type, (void *)(map), &map->block_list);
		}
	}
	else if (block_type == BLOCK_TYPE__LARGE)
		show_large_block_memory(head);
}

void	show_memory(void)
{
	t_context	*context;

	if (!get_context(&context))
		return ;
	show_map_memory(BLOCK_TYPE__TINY, &context->tiny_list_head);
	show_map_memory(BLOCK_TYPE__SMALL, &context->small_list_head);
	show_map_memory(BLOCK_TYPE__LARGE, &context->large_list_head);
}

/*
** malloc() / free()
*/
void	*malloc(size_t size)
{
	t_context		*context;
	t_list			*map_list_head;
	t_map_info		*map_info;
	t_block			*block;
	void			*area;

	//fprintf(stderr, "Coucouc c mon Malloc");

	if (!size)
		return (NULL);
	if (!get_context(&context))
		return (NULL);

	if (size < context->small_info.user_block_size)
	{
		if (size < context->tiny_info.user_block_size)
		{
			map_list_head = &context->tiny_list_head;
			map_info = &context->tiny_info;
		}
		else
		{
			map_list_head = &context->small_list_head;
			map_info = &context->small_info;
		}
		block = block_retrieve(map_info, map_list_head);
		if (block == NULL)
		{
			LOG_ERROR("block_retrieve() failed %s", "");
			return (NULL);
		}
	}
	else
	{
		block = internal_mmap(sizeof(t_block) + size);
		if (block == NULL)
		{
			LOG_ERROR("internal_mmap() failed block_size %zu", size);
			return (NULL);
		}
		list_add_tail(&block->list, &context->large_list_head);
		block->type = BLOCK_TYPE__LARGE;
	}
	block->state = BLOCK_STATE__USED;
	block->size = size;
	area = BLOCK_TO_AREA_OFFSET(block);
	return (area);
}

void	free(void *ptr)
{
	t_block	*block;

	//fprintf(stderr, "Coucouc c mon Free");
	if (ptr == NULL)
		return ;
	block = AREA_TO_BLOCK_OFFSET(ptr);
	if (!block_return(block))
		LOG_ERROR("block_return() failed %s", "");
}

/*
** calloc()
*/
void	*calloc(size_t nmemb, size_t size)
{
	size_t	alloc_size;
	void	*alloc;

	alloc_size = nmemb * size;
	if (alloc_size == 0)
		return (NULL);
	alloc = malloc(alloc_size);
	if (alloc == NULL)
		return (NULL);
	ft_bzero(alloc, alloc_size);
	return (alloc);
}

/*
** realloc()
*/
void	*realloc(void *ptr, size_t size)
{
	t_block	*ptr_block;
	t_block	*new_block;
	void	*new_area;

	if (ptr == NULL)
		return (NULL);
	ptr_block = AREA_TO_BLOCK_OFFSET(ptr);
	if (size < ptr_block->size)
	{
		free(ptr);
		return (NULL);
	}
	new_area = malloc(size);
	if (new_area == NULL)
	{
		free(ptr);
		return (NULL);
	}
	new_block = AREA_TO_BLOCK_OFFSET(new_area);
	ft_memcpy(new_area, ptr, ptr_block->size);
	free(ptr);
	return (new_area);
}
