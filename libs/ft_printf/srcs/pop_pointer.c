/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 pop_pointer.c										:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: abombard <marvin@42.fr>					+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2016/03/31 14:03:59 by abombard		   #+#	  #+#			  */
/*	 Updated: 2016/03/31 14:06:47 by abombard		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "ft_printf.h"

static void			base_init(char *base)
{
	base[0] = '0';
	base[1] = '1';
	base[2] = '2';
	base[3] = '3';
	base[4] = '4';
	base[5] = '5';
	base[6] = '6';
	base[7] = '7';
	base[8] = '8';
	base[9] = '9';
	base[10] = 'a';
	base[11] = 'b';
	base[12] = 'c';
	base[13] = 'd';
	base[14] = 'e';
	base[15] = 'f';
}

static uint8_t		get_nbr_hex(t_buffer *dst, unsigned long long nbr)
{
	char				base[16];
	char				tmp[14];
	int					x;

	base_init(base);
	dst->size = 12;
	tmp[13] = 0;
	x = 12;
	while (nbr > 0)
	{
		if (x == 0)
		{
			ft_putendl_fd("FT_PRINTF: Buffer overflow", 2);
			return (0);
		}
		tmp[x--] = base[nbr % sizeof(base)];
		nbr /= sizeof(base);
	}
	ft_strcpy((char *)dst->data, tmp + x + 1);
	dst->size = ft_strlen((char *)dst->data);
	return (1);
}

uint8_t				case_p(t_buffer *dst, va_list ap, t_flags *flag)
{
	unsigned long				pt;

	pt = va_arg(ap, unsigned long);
	if (pt == 0)
	{
		dst->size = sizeof("(nil)");
		ft_memcpy(dst->data, "(nil)", dst->size);
	}
	if (!get_nbr_hex(dst, pt))
		return (0);
	flag->type = INTERNAL_PRINTF_POINTER;
	return (1);
}
