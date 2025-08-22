/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:43:22 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/22 13:44:01 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_arr_size(char **arr)
{
	size_t	size;

	size = 0;
	while (arr[size])
		size++;
	return (size);
}

size_t	ft_lst_size(t_token *tokens)
{
	size_t	size;

	size = 0;
	while (tokens)
	{
		size++;
		tokens = tokens->next;
	}
	return (size);
}

void	ft_merge(t_shell *shell, char ***arr, size_t lst_size)
{
	char	**new_arr;
	size_t	i;
	t_token	*tmp;

	tmp = shell->lexer->tmp;
	new_arr = malloc(sizeof(char *) * (lst_size + 2));
	if (!new_arr)
		ft_critical_error(shell);
	i = 0;
	new_arr[i] = ft_strdup_safe(shell, (*arr)[i]);
	while (tmp)
	{
		new_arr[++i] = ft_strdup_safe(shell, tmp->data);
		tmp = tmp->next;
	}
	new_arr[lst_size + 1] = NULL;
	ft_free_arr(*arr);
	*arr = new_arr;
}
