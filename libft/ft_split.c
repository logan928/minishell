/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:34:07 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/22 12:34:17 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wc(char const *s, char c)
{
	size_t	wc;

	wc = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			wc++;
			while (*s && *s != c)
				s++;
		}
	}
	return (wc);
}

static	size_t	ft_elalloc(char **arr, size_t i, const char *s, char c)
{
	size_t	word_len;

	word_len = 0;
	while (s[word_len] && s[word_len] != c)
		word_len++;
	arr[i] = (char *)malloc(sizeof(char) * (word_len + 1));
	if (!arr[i])
	{
		while (i-- > 0)
			free(arr[i]);
		free(arr);
		return (0);
	}
	ft_strlcpy(arr[i], s, word_len + 1);
	return (word_len);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	arr = (char **)malloc(sizeof(char *) * (ft_wc(s, c) + 1));
	if (!arr)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			s += ft_elalloc(arr, i, s, c);
			if (!arr)
				return (NULL);
			i++;
		}
	}
	arr[i] = NULL;
	return (arr);
}
