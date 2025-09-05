/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:43:22 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:16:34 by mkugan           ###   ########.fr       */
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

void	ft_merge(t_shell *shell, char ***arr, size_t lst_size, int is_cmd)
{
	char	**new_arr;
	size_t	i;
	t_token	*tmp;

	tmp = shell->lexer->tmp;
	new_arr = fts_malloc(shell, sizeof(char *) * (lst_size + 1 + is_cmd));
	if (!new_arr)
		ft_critical_error(shell);
	i = 0;
	if (is_cmd)
		new_arr[0] = fts_strdup(shell, (*arr)[i++]);
	while (tmp)
	{
		new_arr[i++] = fts_strdup(shell, tmp->data);
		tmp = tmp->next;
	}
	if (is_cmd)
		new_arr[lst_size + 1] = NULL;
	else
		new_arr[lst_size] = NULL;
	ft_free_arr(*arr);
	*arr = new_arr;
}
