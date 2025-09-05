/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:19:02 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:17:02 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_tokenize_op(t_shell *shell, t_cursor *c)
{
	t_token			*next;
	t_token_kind	kind;
	size_t			len;
	char			*data;

	kind = ft_get_token_kind(&(shell->input[c->cur]));
	len = ft_get_operator_length(kind);
	data = fts_strndup(shell, &shell->input[c->cur], len);
	next = fts_new_token(shell, kind, data);
	ft_add_token(&(shell->lexer->tokens), next);
	c->cur += len;
}

void	ft_tokenize_nl(t_shell *shell)
{
	t_token	*next;
	char	*data;

	data = fts_strdup(shell, "newline");
	next = fts_new_token(shell, NL, data);
	ft_add_token(&shell->lexer->tokens, next);
}

void	ft_tokenize_word(t_shell *shell, t_cursor *c)
{
	t_token	*next;
	char	*input;
	char	*data;

	input = shell->input;
	data = fts_strndup(shell, &input[c->start], c->cur - c->start);
	next = fts_new_token(shell, WORD, data);
	ft_add_token(&shell->lexer->tokens, next);
}

void	lex(t_shell *s, char *in)
{
	t_cursor	c;

	c = (t_cursor){0, 0, 0};
	while (in[c.cur])
	{
		while (ft_isspace(in[c.cur]) && c.quote == 0)
			c.cur++;
		if (ft_is_operator_char(in[c.cur]) && c.quote == 0)
			ft_tokenize_op(s, &c);
		else if (in[c.cur])
		{
			c.start = c.cur;
			while (in[c.cur] && (c.quote || ft_is_normal_char(in[c.cur])))
			{
				if (ft_isquote(in[c.cur]) && !c.quote)
					c.quote = in[c.cur];
				else if (ft_isquote(in[c.cur]) && c.quote == in[c.cur])
					c.quote = 0;
				c.cur++;
			}
			ft_tokenize_word(s, &c);
		}
	}
	ft_tokenize_nl(s);
}
