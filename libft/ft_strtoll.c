#include "libft.h"

static int	ft_get_digit(int c, int base)
{
	const char	*digits;
	int			i;

	digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	i = 0;
	while (i < base)
	{
		if (digits[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

static void	ft_set_sign_base(const char **str, int *negative, int *base)
{
	while (**str && ft_isspace(**str))
		(*str)++;
	if (**str && (**str == '-' || **str == '+'))
	{
		if (**str == '-')
			*negative = 1;
		(*str)++;
	}
	if (*base == 0)
	{
		if (**str == '0' && (*str[1] == 'x' || *str[1] == 'X'))
		{
			*base = 16;
			*str += 2;
		}
		else if (**str == '0')
		{
			*base = 8;
			*str += 1;
		}
		else
			*base = 10;
	}
	while (**str == '0')
		*str += 1;
}

static long long	ft_return_max(char **str_end, int negative, const char *str)
{
	if (str_end)
		*str_end = (char *) str;
	if (negative)
		return (LLONG_MIN);
	else
		return (LLONG_MAX);
}

long long	ft_strtoll(const char *str, char **str_end, int base)
{
	long long	ret;
	int			digit;
	int			negative;

	if (!str || base < 0 || base == 1 || base > 36)
		return (0);
	ret = 0;
	negative = 0;
	ft_set_sign_base(&str, &negative, &base);
	while (*str)
	{
		digit = ft_get_digit(ft_tolower(*str), base);
		if (digit == -1)
			break ;
		if (ret > (LLONG_MAX - digit) / base)
			return (ft_return_max(str_end, negative, str));
		ret = ret * base + digit;
		str++;
	}
	if (str_end)
		*str_end = (char *) str;
	if (negative)
		return (-ret);
	else
		return (ret);
}
