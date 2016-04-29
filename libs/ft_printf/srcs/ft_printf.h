/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 12:45:28 by abombard          #+#    #+#             */
/*   Updated: 2016/04/19 12:52:09 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdint.h>
# include <stdarg.h>
# include <unistd.h>
# include "libft.h"

# define PRINTF_BUFFER_SIZE_MAX					2048
# define INTERNAL_DATA_SIZE_MAX					512

# define INTERNAL_PRINTF_CHAR					1
# define INTERNAL_PRINTF_STRING					2
# define INTERNAL_PRINTF_INT					3
# define INTERNAL_PRINTF_UOCTAL					4
# define INTERNAL_PRINTF_UDECIMAL				5
# define INTERNAL_PRINTF_UHEXA_MAJ				6
# define INTERNAL_PRINTF_UHEXA_MIN				7
# define INTERNAL_PRINTF_POINTER				8

typedef struct	s_buffer
{
	uint8_t			data[INTERNAL_DATA_SIZE_MAX];
	uint32_t		size;
}				t_buffer;

typedef struct	s_flags
{
	uint8_t			space;
	uint32_t		precision;
	uint32_t		field;
	uint8_t			zero;
	uint8_t			diez;
	uint8_t			plus;
	uint8_t			moins;
	uint8_t			hh;
	uint8_t			h;
	uint8_t			ll;
	uint8_t			l;
	uint8_t			j;
	uint8_t			z;
	uint32_t		type;
}				t_flags;

uint8_t			case_nbr(t_buffer *dst, va_list ap, t_flags *flag);
uint8_t			case_p(t_buffer *dst, va_list ap, t_flags *flag);
uint8_t			case_x(t_buffer	*dst, va_list ap, t_flags *flag);
uint8_t			case_x_maj(t_buffer	*dst, va_list ap, t_flags *flag);
uint8_t			case_o(t_buffer *dst, va_list ap, t_flags *flag);
uint8_t			case_u(t_buffer *dst, va_list ap, t_flags *flag);
uint8_t			case_c(t_buffer *dst, va_list ap, t_flags *flag);
uint8_t			case_s(t_buffer *dst, va_list ap, t_flags *flag);

uint8_t			get_char(t_buffer *dst, uint32_t ch);

uint8_t			flag_get_main(char *fmt, uint32_t *len, t_flags *flag);

uint8_t			flag_apply_main(t_buffer *dst, t_flags *flag);

uint8_t			flag_apply_precision(t_buffer *dst, t_flags *flag);
uint8_t			flag_apply_field(t_buffer *dst, t_flags *flag);
uint8_t			flag_apply_spaceplus(t_buffer *dst, t_flags *flag);
uint8_t			flag_apply_diez(t_buffer *dst, uint32_t type);

uint8_t			check_errors(const char *fmt);
uint8_t			internal_printf(int fd, char *fmt, va_list ap, uint32_t *ret);

#endif
