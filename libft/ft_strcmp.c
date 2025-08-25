/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:32:00 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 22:32:09 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2, int ignore_case)
{
	if (ignore_case)
	{
		while (*s1 && ft_tolower((unsigned char)*s1) == ft_tolower((unsigned char)*s2))
		{
			s1++;
			s2++;
		}
		return (ft_tolower((unsigned char)*s1) - ft_tolower((unsigned char)*s2));
	}
	while (*s1 && (unsigned char)*s1 == (unsigned char)*s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
