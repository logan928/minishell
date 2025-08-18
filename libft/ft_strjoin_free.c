/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:41:27 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/18 10:43:34 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_str_size(char *s)
{
	if (s)
		return (ft_strlen(s));
	else
		return (0);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	size1;
	size_t	size2;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	size1 = ft_str_size(s1);
	size2 = ft_str_size(s2);
	res = malloc(size1 + size2 + 1);
	if (!res)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	if (s1)
		ft_memcpy(res, s1, size1);
	if (s2)
		ft_memcpy(res + size1, s2, size2);
	res[size1 + size2] = '\0';
	free(s1);
	free(s2);
	return (res);
}
