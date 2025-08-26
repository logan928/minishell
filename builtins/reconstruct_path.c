/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reconstruct_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:40:59 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/26 21:40:59 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**ft_stack_to_array(t_stack *st, size_t *out_size)
{
	char	**parts;
	size_t	i;
	size_t	size;

	size = st->size;
	parts = malloc(sizeof(char *) * size);
	if (!parts)
		return (NULL);
	i = 0;
	while (i < size)
		parts[i++] = ft_stack_pop(st);
	*out_size = size;
	return (parts);
}

static size_t	ft_calc_len(char **parts, size_t size)
{
	size_t	len;
	size_t	i;

	len = 1;
	i = 0;
	while (i < size)
		len += ft_strlen(parts[i++]);
	len += size;
	return (len);
}

static char	*ft_build_path(char **parts, size_t size, size_t len)
{
	char	*res;

	res = malloc(len);
	if (!res)
	{
		ft_free_arr(parts);
		return (NULL);
	}
	res[0] = '/';
	res[1] = '\0';
	while (size > 0)
	{
		ft_strlcat(res, parts[size - 1], len);
		if (size > 1)
			ft_strlcat(res, "/", len);
		size--;
	}
	free(parts);
	return (res);
}

char	*ft_reconstruct_path(t_stack *st)
{
	size_t	size;
	char	**parts;
	size_t	len;
	char	*res;

	if (st == NULL || st->size == 0)
		return (ft_strdup("/"));
	parts = ft_stack_to_array(st, &size);
	if (!parts)
		return (NULL);
	len = ft_calc_len(parts, size);
	res = ft_build_path(parts, size, len);
	return (res);
}
