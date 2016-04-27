#include "internal_malloc.h"

#include <sys/mman.h>

#define BLOCK_COUNT 100
bool	mem_handle__initialize(t_mem_handle	*mem_handle,
							   const size_t map_size)
{
	size_t	block_size;
	size_t	block_offset;
	t_block	*block;

	INIT_LIST_HEAD(&mem_handle->fifo_free);
	INIT_LIST_HEAD(&mem_handle->fifo_used);
	if (mem_handle->mem_type == MEM_TYPE__TINY ||
		mem_handle->mem_type == MEM_TYPE__SMALL)
	{
		block_size = map_size / BLOCK_COUNT;
		mem_handle->block_count = BLOCK_COUNT;
		mem_handle->block_size = block_size - sizeof(t_block);
		mem_handle->map_size = map_size;
		mem_handle->map = mmap(0,
							   mem_handle->map_size,
							   PROT_READ | PROT_WRITE,
							   MAP_ANON | MAP_PRIVATE,
							   -1, 0);
		ASSERT(mem_handle->map != NULL);

		block_offset = 0;
		while (block_offset < mem_handle->map_size)
		{
			block = (t_block *)(mem_handle->map + block_offset);
			block->size = 0;
			block->mem_type = mem_handle->mem_type;
			block->fifo_type = FIFO_TYPE__FREE;
			list_add(&block->fifo_list, &mem_handle->fifo_free);
			block_offset += block_size;
		}
	}
	return (TRUE);
}

bool	mem_handle__finalize(t_mem_handle *mem_handle)
{
	t_block	*block;
	t_list	*head;
	t_list	*pos;
	t_list	*next;

	if (mem_handle->mem_type == MEM_TYPE__TINY ||
		mem_handle->mem_type == MEM_TYPE__SMALL)
	{
		ASSERT(munmap(mem_handle->map, mem_handle->map_size) == 0);
	}
	else if (mem_handle->mem_type == MEM_TYPE__LARGE)
	{
		head = &mem_handle->fifo_used;
		next = head->next;
		while ((pos = next) && pos != head
			   && (next = next->next))
		{
			block = CONTAINER_OF(pos, t_block, fifo_list);
			LOG_WARNING("munmap() block %p size %zu", block, block->size);
			if (munmap(block, block->size) != 0)
			{
				LOG_ERROR("munmap() failed block %p size %zu", block, block->size);
				return (FALSE);
			}
		}
	}
	return (TRUE);
}
