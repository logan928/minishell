/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:19:02 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 13:23:20 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_is_operator_char(char c)
{
	const char	*chars;

	chars = "|&<>()";
	while (*chars)
	{
		if (c == *chars)
			return (1);
		chars++;
	}
	return (0);
}

static int	ft_is_normal_char(char c)
{
	return (!ft_isspace(c) && !ft_is_operator_char(c));
}

static t_token_kind	ft_get_token_kind(const char *s)
{
	if (s[0] == '|' && s[1] == '|')
		return (OR_IF);
	else if (s[0] == '|')
		return (PIPE);
	else if (s[0] == '&' && s[1] == '&')
		return (AND_IF);
	else if (s[0] == '<' && s[1] == '<')
		return (DLESS);
	else if (s[0] == '<')
		return (LESS);
	else if (s[0] == '>' && s[1] == '>')
		return (DGREAT);
	else if (s[0] == '>')
		return (GREAT);
	else if (s[0] == '(')
		return (L_PAREN);
	else if (s[0] == ')')
		return (R_PAREN);
	return (WORD);
}

static int	ft_get_operator_length(t_token_kind kind)
{
	if (kind == OR_IF || kind == AND_IF || kind == DLESS || kind == DGREAT)
		return (2);
	return (1);
}

void	ft_tokenize_op(t_shell *shell, t_cursor *c)
{
	t_token			*next;
	t_token_kind	kind;
	size_t			len;
	char			*data;

	kind = ft_get_token_kind(&(shell->input[c->cur]));
	len = ft_get_operator_length(kind);
	data = ft_strndup_safe(shell, &shell->input[c->cur], len);
	next = ft_new_token_safe(shell, kind, data);
	ft_add_token(&(shell->lexer->tokens), next);
	c->cur += len;
}

void	ft_tokenize_nl(t_shell *shell)
{
	t_token	*next;
	char	*data;

	data = ft_strdup_safe(shell, "newline");
	next = ft_new_token_safe(shell, NL, data);
	ft_add_token(&shell->lexer->tokens, next);
}

void	ft_tokenize_word(t_shell *shell, t_cursor *c)
{
	t_token	*next;
	char	*input;
	char	*data;

	input = shell->input;
	data = ft_strndup_safe(shell, &input[c->start], c->cur - c->start);
	next = ft_new_token_safe(shell, WORD, data);
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
				else if (ft_isquote(in[c.cur]) && c.quote)
					c.quote = 0;
				c.cur++;
			}
			ft_tokenize_word(s, &c);
		}
	}
	ft_tokenize_nl(s);
}
