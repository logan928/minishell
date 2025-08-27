/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_arr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:39:26 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/27 10:39:28 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_realloc_arr(char **old)
{
	size_t	old_size;
	size_t	new_size;
	char	**new;
	size_t	i;

	old_size = 0;
	while (old[old_size] != NULL)
		old_size++;
	new_size = old_size + 2;
	new = malloc(sizeof(char *) * new_size);
	if (new == NULL)
		return (NULL);
	i = 0;
	while (i < old_size)
	{
		new[i] = old[i];
		i++;
	}
	new[old_size] = NULL;
	new[old_size + 1] = NULL;
	free(old);
	return (new);
}
