/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:55 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:12:53 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_split_ifs(t_shell *shell, char *s)
{
	t_token		*new_token;
	t_cursor	c;

	c = (t_cursor){0, 0, 0};
	while (s[c.cur])
	{
		while (ft_isspace(s[c.cur]) && c.quote == 0)
			c.cur++;
		c.start = c.cur;
		while (s[c.cur] && ((ft_isspace(s[c.cur]) && c.quote)
				|| !ft_isspace(s[c.cur])))
		{
			if (ft_isquote(s[c.cur]) && !c.quote)
				c.quote = s[c.cur];
			else if (ft_isquote(s[c.cur]) && c.quote == s[c.cur])
				c.quote = 0;
			c.cur++;
		}
		new_token = ft_new_token(WORD, fts_strndup(shell,
					&s[c.start], c.cur - c.start));
		ft_add_token(&shell->lexer->tmp, new_token);
	}
}

void	ft_field_splitting(t_shell *shell, char ***arr, size_t idx)
{
	size_t	lst_size;
	size_t	arr_size;

	shell->lexer->tmp = NULL;
	while ((*arr)[idx])
	{
		ft_split_ifs(shell, (*arr)[idx]);
		idx++;
	}
	lst_size = ft_lst_size(shell->lexer->tmp);
	arr_size = ft_arr_size(*arr);
	if (lst_size != arr_size - 1)
		ft_merge(shell, arr, lst_size, 0);
	ft_free_tokens(shell->lexer->tmp);
	shell->lexer->tmp = NULL;
}
