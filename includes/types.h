/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 types.h											:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: abombard <marvin@42.fr>					+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2016/04/01 16:11:20 by abombard		   #+#	  #+#			  */
/*	 Updated: 2016/04/01 16:14:46 by abombard		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <unistd.h>
# include <stdint.h>
# include <inttypes.h>
# include <stdbool.h>

# define FALSE			0
# define TRUE			1

typedef char			t_byte;

typedef struct {
	size_t				size;
	char				*bytes;
}	t_buffer;

# define STRING_TO_BUFFER(str)	(t_buffer){ (size_t)(sizeof(str) - 1), (char *)str }
# define BUFFER_INIT(size, str)	(t_buffer){ (size_t)size, (char *)str }

typedef int				t_failure_code;
# define PRI_FAILURE_CODE				"d"

# define FAILURE_CODE_SUCCESS			1
# define FAILURE_CODE_UNDEFINED			2
# define FAILURE_CODE_NONE				3
# define FAILURE_CODE_FATAL				4
# define FAILURE_CODE_NULL_PARAMETER	5

#endif
