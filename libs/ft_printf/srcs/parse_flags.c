
#include "ft_printf.h"

static uint32_t	get_precision(char *fmt, uint32_t *i)
{
	uint32_t	ret;

	*i += 1;
	ret = ft_atoi(&fmt[*i]);
	while (ft_isdigit(fmt[*i + 1]))
	{
		*i += 1;
	}
	return (ret);
}

static uint32_t	get_field(char *fmt, uint32_t *i)
{
	uint32_t	ret;

	ret = ft_atoi(&fmt[*i]);
	while (ft_isdigit(fmt[*i + 1]))
	{
		*i += 1;
	}
	return (ret);
}

static void	get_modifier(char *fmt, uint32_t *len, t_flags *flag)
{
	if (fmt[*len] == 'h')
	{
		if (fmt[*len + 1] == 'h')
		{
			flag->hh = 1;
			*len += 1;
		}
		else
			flag->h = 1;
	}
	else if (fmt[*len] == 'l')
	{
		if (fmt[*len + 1] == 'l')
		{
			flag->ll = 1;
			*len += 1;
		}
		else
			flag->l = 1;
	}
	else if (fmt[*len] == 'j')
		flag->j = 1;
	else if (fmt[*len] == 'z')
	{
		flag->z = 1;
		if (fmt[*len + 1] == 'u')
			*len += 1;
	}
}

static uint8_t	get_flag(char *fmt, uint32_t *len, t_flags *flag)
{
	if (fmt[*len] == ' ')
		flag->space = 1;
	else if (fmt[*len] == '.')
		flag->precision = get_precision(fmt, &*len);
	else if (fmt[*len] == '0')
		flag->zero = 1;
	else if (ft_isdigit(fmt[*len]))
		flag->field = get_field(fmt, &*len);
	else if (fmt[*len] == '#')
		flag->diez = 1;
	else if (fmt[*len] == '+')
		flag->plus = 1;
	else if (fmt[*len] == '-')
		flag->moins = 1;
	else
		return (0);
	return (1);
}

uint8_t		flag_get_main(char *fmt, uint32_t *len, t_flags *flag)
{
	if (get_flag(fmt, len, flag))
		return (1);
	if (fmt[*len] != 'h' && fmt[*len] != 'l' &&
		fmt[*len] != 'j' && fmt[*len] != 'z')
		return (0);
	get_modifier(fmt, len, flag);
	return (0);
}
