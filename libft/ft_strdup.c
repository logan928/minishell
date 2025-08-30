/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:26:16 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/07 17:26:31 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*new;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s);
	new = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < s_len)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
