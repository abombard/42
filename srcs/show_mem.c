#include "internal_malloc.h"
#include "ft_printf.h"

/*
 ** show_mem()
 */
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

