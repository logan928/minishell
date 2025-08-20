/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:46 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 17:46:12 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_append_before_open_quote(t_shell *shell, t_token *t, char **clean)
{
	int		start;
	int		pos;
	char	*tmp;

	pos = shell->lexer->pos;
	start = shell->lexer->start;
	shell->lexer->quote = t->data[pos];
	if (pos - start > 0)
	{
		tmp = ft_strndup_safe(shell, &t->data[start], pos - start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	shell->lexer->pos++;
	shell->lexer->start = shell->lexer->pos;
}

void	ft_append_before_close_quote(t_shell *shell, t_token *t, char **clean)
{
	int		start;
	int		pos;
	char	*tmp;

	pos = shell->lexer->pos;
	start = shell->lexer->start;
	shell->lexer->quote = 0;
	if (pos - start > 0)
	{
		tmp = ft_strndup_safe(shell, &t->data[start], pos - start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	shell->lexer->pos++;
	shell->lexer->start = shell->lexer->pos;
}

void	ft_append_rest(t_shell *shell, t_token *t, char **clean)
{
	int		start;
	int		pos;
	char	*tmp;

	pos = shell->lexer->pos;
	start = shell->lexer->start;
	if (pos - start > 0)
	{
		tmp = ft_strndup_safe(shell, &t->data[start], pos - start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	free(t->data);
	if (!*clean)
		t->data = ft_strdup_safe(shell, "''");
	else
		t->data = *clean;
}

void	ft_quote_removal(t_shell *shell)
{
	t_token	*t;
	char	*clean;

	t = shell->lexer->tokens;
	while (t)
	{
		if (t->token_kind == WORD)
		{
			ft_reset_lexer_cursor(shell->lexer);
			clean = NULL;
			while (t->data[shell->lexer->pos])
			{
				if (ft_isquote(t->data[shell->lexer->pos])
					&& !shell->lexer->quote)
					ft_append_before_open_quote(shell, t, &clean);
				else if (ft_isquote(t->data[shell->lexer->pos])
					&& shell->lexer->quote == t->data[shell->lexer->pos])
					ft_append_before_close_quote(shell, t, &clean);
				else
					shell->lexer->pos++;
			}
			ft_append_rest(shell, t, &clean);
		}
		t = t->next;
	}
}
