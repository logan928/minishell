/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:19:02 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/18 10:42:53 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_is_operator_char(char c)
{
	const char	*chars;

	chars = "|&;<>()";
	while (*chars)
	{
		if (c == *chars)
			return (1);
		chars++;
	}
	return (0);
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

void	ft_tokenize_op(t_shell *shell)
{
	t_token			*next_token;
	t_token_kind	kind;
	size_t			len;
	
	kind = ft_get_token_kind(&(shell->input[shell->lexer->pos]));
	len = ft_get_operator_length(kind);
	next_token = ft_new_token(kind, ft_strndup(&(shell->input[shell->lexer->pos]), len));
	if (!next_token)
		ft_critical_error(shell);
	ft_add_token(&(shell->lexer->tokens), next_token);
	shell->lexer->pos += len;
}

void	ft_tokenize_nl(t_shell *shell)
{
	t_token			*next_token;

	next_token = ft_new_token(NL, ft_strdup("newline"));
	if (!next_token)
		ft_critical_error(shell);
	ft_add_token(&shell->lexer->tokens, next_token);
}

void	ft_tokenize_word(t_shell *shell)
{
	t_token	*next_token;
	t_lexer	*l;
	char	*input;

	l = shell->lexer;
	input = shell->input;
	next_token = ft_new_token(WORD, ft_strndup(&input[l->start], l->pos - l->start));
	if (!next_token)
		ft_critical_error(shell);
	ft_add_token(&l->tokens, next_token);
}

void	lex(t_shell *shell)
{
	t_lexer *lexer;
	char	*input;

	lexer = shell->lexer;
	input = shell->input;
	while (input[lexer->pos])
	{
		while (ft_isspace(input[lexer->pos]) && lexer->quote == 0)
			lexer->pos++;
		if (ft_is_operator_char(input[lexer->pos]) && lexer->quote == 0)
			ft_tokenize_op(shell);
		else if (input[lexer->pos])
		{
			lexer->start = lexer->pos;
			while (input[lexer->pos]
				&& ((ft_isspace(input[lexer->pos]) && lexer->quote)
				|| (ft_is_operator_char(input[lexer->pos]) && lexer->quote)
				|| (!ft_isspace(input[lexer->pos]) && !ft_is_operator_char(input[lexer->pos]))))
			{
				if (ft_isquote(input[lexer->pos]) && !lexer->quote)
					lexer->quote = input[lexer->pos];
				else if (ft_isquote(input[lexer->pos]) && lexer->quote)
					lexer->quote = 0;
				lexer->pos++;
			}
			ft_tokenize_word(shell);
		}
	}
	ft_tokenize_nl(shell);
}
