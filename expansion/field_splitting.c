/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:55 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/18 13:10:45 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_split_ifs(char *s, t_shell *shell)
{
	t_token	*split;
	t_token *new_token;
	int		pos;
	int		quote;
	int		start;

	split = NULL;
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
		new_token = ft_new_token(WORD, ft_strndup(&s[start], pos - start));
		if (!new_token)
		{
			ft_free_tokens(split);
			ft_critical_error(shell);
		}
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
			split = ft_split_ifs(t->data, shell);
			if (split && split->next != NULL)
			{
				free(t->data);
				t->data = ft_strdup(split->data);
				tmp = split;
				split = split->next;
				free(tmp->data);
				free(tmp);
				tmp = split;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = t->next;
				t->next = split;
			}
			else if (split)
			{
				ft_free_tokens(split);
			}
		}
		t = t->next;
	}
}
