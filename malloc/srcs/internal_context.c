#include "internal_malloc.h"

bool	internal_context__initialize(t_internal_context *context)
{
	int		page_size;
	size_t	tiny_map_size;
	size_t	small_map_size;

	page_size = getpagesize();
	ASSERT(page_size > 0);
	tiny_map_size = (size_t)page_size * 5;
	small_map_size = (size_t)(page_size * 10);
	ASSERT(mem_handle__initialize(&context->tiny, tiny_map_size));
	ASSERT(mem_handle__initialize(&context->small, small_map_size));
	ASSERT(mem_handle__initialize(&context->large, 0));
	context->is_initialized = TRUE;
	return (TRUE);
}

void	internal_context__finalize(void)
{
	t_internal_context	*context;

	if (!get_internal_context(&context))
	{
		LOG_ERROR("get_internal_context() failed %s", "");
	}
	LOG_DEBUG("Free Tiny %s", "");
	if (!mem_handle__finalize(&context->tiny))
	{
		LOG_ERROR("mem_handle__finalize() failed on %s", "tiny");
	}
	LOG_DEBUG("Free Small %s", "");
	if (!mem_handle__finalize(&context->small))
	{
		LOG_ERROR("mem_handle__finalize() failed on %s", "small");
	}
	LOG_DEBUG("Free Large %s", "");
	if (!mem_handle__finalize(&context->large))
	{
		LOG_ERROR("mem_handle__finalize() failed on %s", "large");
	}
	context->is_initialized = FALSE;
}

bool	get_internal_context(t_internal_context **out_context)
{
	static t_internal_context	context = {
		.is_initialized = FALSE,
		.tiny = { .mem_type = MEM_TYPE__TINY },
		.small = { .mem_type = MEM_TYPE__SMALL },
		.large = { .mem_type = MEM_TYPE__LARGE },
	};

	if (context.is_initialized == FALSE)
	{
		ASSERT(internal_context__initialize(&context));
		ASSERT(!atexit(&internal_context__finalize));
	}
	*out_context = &context;
	return (TRUE);
}
