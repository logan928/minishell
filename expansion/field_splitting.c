/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:55 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 17:17:10 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_split_ifs(t_shell *shell, char *s)
{
	t_token	*new_token;
	int		pos;
	int		quote;
	int		start;

	pos = 0;
	quote = 0;
	while (s[pos])
	{
		while (ft_isspace(s[pos]) && quote == 0)
			pos++;
		start = pos;
		while (s[pos] && ((ft_isspace(s[pos]) && quote) || !ft_isspace(s[pos])))
		{
			if (ft_isquote(s[pos]) && !quote)
				quote = s[pos];
			else if (ft_isquote(s[pos]) && quote == s[pos])
				quote = 0;
			pos++;
		}
		new_token = ft_new_token(WORD, ft_strndup_safe(shell,
					&s[start], pos - start));
		ft_add_token(&shell->lexer->tmp, new_token);
	}
}

void	ft_insert_split(t_shell *shell, t_token *split, t_token *t)
{
	t_token	*tmp;

	free(t->data);
	t->data = ft_strdup_safe(shell, split->data);
	tmp = split;
	split = split->next;
	free(tmp->data);
	free(tmp);
	tmp = split;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = t->next;
	t->next = split;
	shell->lexer->tmp = NULL;
}

void	ft_field_splitting(t_shell *shell, char ***arr)
{
	size_t	i;
	size_t	lst_size;
	size_t	arr_size;

	i = 1;
	shell->lexer->tmp = NULL;
	while ((*arr)[i])
	{
		ft_split_ifs(shell, (*arr)[i]);
		i++;
	}
	lst_size = ft_lst_size(shell->lexer->tmp);
	arr_size = ft_arr_size(*arr);
	if (lst_size != arr_size - 1)
		ft_merge(shell, arr, lst_size);
	ft_free_tokens(shell->lexer->tmp);
	shell->lexer->tmp = NULL;
}
