#include "internal_malloc.h"

bool	block__is_free(const t_block *block)
{
	if (block->size == 0 && block->fifo_type == FIFO_TYPE__FREE)
		return (TRUE);
	LOG_ERROR("block->size %zu block->fifo_type %d", block->size, block->fifo_type);
	return (FALSE);
}

bool	block__is_used(const t_block *block)
{
	if (block->size != 0 && block->fifo_type == FIFO_TYPE__USED)
		return (TRUE);
	LOG_ERROR("block->size %zu block->fifo_type %d", block->size, block->fifo_type);
	return (FALSE);
}

bool	block_to_mem_handle(t_internal_context *context,
							const t_block *block,
							t_mem_handle **mem_handle)
{
	if (block->mem_type == MEM_TYPE__TINY)
	{
		*mem_handle = &context->tiny;
	}
	else if (block->mem_type == MEM_TYPE__SMALL)
	{
		*mem_handle = &context->small;
	}
	else if (block->mem_type == MEM_TYPE__LARGE)
	{
		*mem_handle = &context->large;
	}
	else
	{
		LOG_ERROR("block->mem_type %d", block->mem_type);
		return (FALSE);
	}
	return (TRUE);
}

#include <sys/mman.h>
bool	mem_handle__block_alloc_large(t_mem_handle *mem_handle,
									  const size_t size,
									  t_block **out_block)
{
	t_block	*block;
	size_t	block_size;

	block_size = size;

	*out_block = NULL;
	block = mmap(0,
				 block_size,
				 PROT_READ | PROT_WRITE,
				 MAP_ANON | MAP_PRIVATE,
				 -1, 0);
	if (block == NULL)
		FATAL("mmap() failed block_size %zu", block_size);
	block->mem_type = MEM_TYPE__LARGE;
	block->size = block_size;
	block->fifo_type = FIFO_TYPE__USED;
	list_add(&block->fifo_list, &mem_handle->fifo_used);

	*out_block = block;
	return (TRUE);
}

bool	mem_handle__block_prealloced(t_mem_handle *mem_handle,
									 const size_t block_size,
									 t_block **out_block)
{
	t_list	*fifo_list;
	t_block	*block;

	*out_block = NULL;

	ASSERT(mem_handle->mem_type == MEM_TYPE__TINY ||
		   mem_handle->mem_type == MEM_TYPE__SMALL);

	if (!list_empty(&mem_handle->fifo_free))
	{
		fifo_list = mem_handle->fifo_free.next;
		block = CONTAINER_OF(fifo_list, t_block, fifo_list);
		ASSERT(mem_handle->mem_type == block->mem_type);
		ASSERT(block__is_free(block));
		block->size = block_size;
		block->fifo_type = FIFO_TYPE__USED;
		list_move(fifo_list, &mem_handle->fifo_used);
		*out_block = block;
	}
	else
	{
		LOG_WARNING("fifo_free is empty %s", "need to grow");
		return (FALSE);
	}

	return (TRUE);
}

void	*internal_malloc(const size_t size)
{
	t_internal_context	*context;
	t_block				*block;
	void				*area;

	if (!get_internal_context(&context))
	{
		LOG_ERROR("get_internal_context() failed %s", "");
		return (NULL);
	}

	if (size < context->tiny.block_size)
	{
		LOG_DEBUG("allocating TINY size %zu", size);
		if (!mem_handle__block_prealloced(&context->tiny, size, &block))
			FATAL("mem_handle__block_prealloced() failed %s", "TINY");
	}
	else if (size < context->small.block_size)
	{
		LOG_DEBUG("allocating SMALL size %zu", size);
		if (!mem_handle__block_prealloced(&context->small, size, &block))
			FATAL("mem_handle__block_prealloced() failed %s", "SMALL");
	}
	else
	{
		LOG_DEBUG("allocating LARGE size %zu", size);
		if (!mem_handle__block_alloc_large(&context->large, size, &block))
			FATAL("mem_handle__block_alloc_large() failed %s", "");
	}

	BLOCK_TO_AREA_OFFSET(block, area);
	LOG_DEBUG("block %p area %p", block, area);

	return (area);
}

void	*ft_malloc(size_t size)
{
	void	*area;

	if (size == 0)
		return (NULL);
	area = internal_malloc(size);
	if (!area)
	{
		LOG_ERROR("internal_malloc() failed size %zu", size);
		return (NULL);
	}
	return (area);
}

bool	mem_handle__block_free(t_mem_handle *mem_handle, t_block *block)
{
	ASSERT(mem_handle->mem_type == block->mem_type);
	if (!block__is_used(block))
	{
		FATAL("block is not used %s", "");
		return (FALSE);
	}
	if (mem_handle->mem_type == MEM_TYPE__TINY ||
		mem_handle->mem_type == MEM_TYPE__SMALL)
	{
		LOG_DEBUG("Freeing %s size %zu", mem_handle->mem_type == MEM_TYPE__TINY ? "TINY" : "SMALL", block->size);
		block->size = 0;
		block->fifo_type = FIFO_TYPE__FREE;
		list_move(&block->fifo_list, &mem_handle->fifo_free);
	}
	else if (mem_handle->mem_type == MEM_TYPE__LARGE)
	{
		LOG_DEBUG("Freeing %s size %zu", "LARGE", block->size);
		list_del(&block->fifo_list);
		ASSERT(munmap(block, block->size) == 0);
	}
	return (TRUE);
}

bool	internal_free(void *area)
{
	t_internal_context	*context;
	t_mem_handle		*mem_handle;
	t_block				*block;

	AREA_TO_BLOCK_OFFSET(area, block);
	LOG_DEBUG("block %p area %p", block, area);

	ASSERT(get_internal_context(&context));
	if (!block_to_mem_handle(context, block, &mem_handle))
		FATAL("block_to_mem_handle() failed %s", "");
	if (!mem_handle__block_free(mem_handle, block))
		FATAL("mem_handle__block_free() failed block %p", block);
	return (TRUE);
}

void	ft_free(void *area)
{
	if (area)
	{
		if (!internal_free(area))
		{
			LOG_ERROR("internal_free() failed area %p", (void *)area);
		}
	}
}

void		*ft_realloc(void *ptr, size_t size)
{
	t_internal_context	*context;
	t_mem_handle		*mem_handle;
	t_block				*block;
	void				*new;

	if (!get_internal_context(&context))
	{
		LOG_ERROR("get_internal_context() failed %s", "");
		return (NULL);
	}

	AREA_TO_BLOCK_OFFSET(ptr, block);
	if (!block__is_used(block))
	{
		LOG_ERROR("invalid ptr addr %p", ptr);
		return (NULL);
	}

	if (!block_to_mem_handle(context, block, &mem_handle))
	{
		LOG_ERROR("block_to_mem_handle() failed %s", "");
		return (NULL);
	}

	new = ft_malloc(size);
	if (!new)
	{
		LOG_ERROR("ft_malloc() failed size %zu", size);
		return (NULL);
	}
	ft_memcpy(new, ptr, block->size);
	ft_free(ptr);
	return (new);
}
