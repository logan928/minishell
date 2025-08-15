/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 11:03:27 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/15 11:03:41 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_size_n(long n)
{
	int	len;

	len = 1;
	if (n < 0)
		len++;
	while (n / 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(long n)
{
	char	*num;
	int		len;
	int		is_negative;

	is_negative = 0;
	len = ft_size_n(n);
	if (n < 0)
		is_negative = 1;
	num = (char *)malloc(sizeof(char) * (len + 1));
	if (!num)
		return (NULL);
	num[len] = '\0';
	while (len--)
	{
		if (n % 10 < 0)
			num[len] = -(n % 10) + '0';
		else
			num[len] = (n % 10) + '0';
		n /= 10;
		if (len == 0 && is_negative)
			num[len] = '-';
	}
	return (num);
}
