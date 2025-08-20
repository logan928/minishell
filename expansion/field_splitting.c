/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:55 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 15:49:48 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_split_ifs(t_token *split, char *s, t_shell *shell)
{
	t_token *new_token;
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
		while (s[pos] && ((ft_isspace(s[pos]) && quote)
			|| !ft_isspace(s[pos])))
		{
			if (ft_isquote(s[pos]) && !quote)
				quote = s[pos];
			else if (ft_isquote(s[pos]) && quote == s[pos])
					quote = 0;
			pos++;
		}
		new_token = ft_new_token(WORD, ft_strndup_safe(shell, &s[start], pos - start));
		ft_add_token(&split, new_token);
	}
	
	return (split);
}

void	ft_field_splitting(t_shell *shell)
{
	t_token	*t;
	t_token	*split;
	t_token	*tmp;

	t = shell->lexer->tokens;
	while (t)
	{
		if (t->token_kind == WORD)
		{
			shell->lexer->split_tmp = NULL;
			shell->lexer->split_tmp = ft_split_ifs(shell->lexer->split_tmp, t->data, shell);
			split = shell->lexer->split_tmp;
			if (split && split->next != NULL)
			{
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
				shell->lexer->split_tmp = NULL;
			}
			else if (split)
			{
				ft_free_tokens(split);
				shell->lexer->split_tmp = NULL;
			}
		}
		t = t->next;
	}
}
