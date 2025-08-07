/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:19:02 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/07 18:06:30 by mkugan           ###   ########.fr       */
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
		return (DLESS);
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

void	lex(t_lexer *lexer)
{
	t_token			*next_token;
	t_token_kind	kind;
	size_t			len;
	char			*op;

	while (lexer->src[lexer->pos])
	{
		if (ft_isspace(lexer->src[lexer->pos]))
		{
			lexer->pos++;
			continue ;
		}
		else if (ft_isquote(lexer->src[lexer->pos]))
		{
			lexer->quote = lexer->src[lexer->pos];
			lexer->start = lexer->pos++;
			while (lexer->src[lexer->pos] && lexer->src[lexer->pos] != lexer->quote)
				lexer->pos++;
			if (lexer->src[lexer->pos] == lexer->quote)
				lexer->pos++;
			lexer->word = ft_strndup(&lexer->src[lexer->start], lexer->pos - lexer->start);
			if (!lexer->word)
			{
				perror("ft_strndup | TODO: clean memory, refactor");
				exit(EXIT_FAILURE);
			}
			next_token = ft_new_token(WORD, lexer->word);
			if (!next_token)
			{
				perror("ft_new_token | TODO: clean memory, refactor");
				exit(EXIT_FAILURE);
			}
			ft_add_token(&lexer->tokens, next_token);
			if (lexer->src[lexer->pos] == lexer->quote)
				lexer->pos++;
			lexer->quote = 0;
		}
		else if (ft_is_operator_char(lexer->src[lexer->pos]))
		{
			kind = ft_get_token_kind(&lexer->src[lexer->pos]);
			len = ft_get_operator_length(kind);
			op = ft_strndup(&lexer->src[lexer->pos], len);
			if (!op)
			{
				perror("ft_strndup | TODO: clean memory, refactor");
				exit(EXIT_FAILURE);
			}
			next_token = ft_new_token(kind, op);
			if (!next_token)
			{
				perror("ft_new_token | TODO: clean memory, refactor");
				exit(EXIT_FAILURE);
			}
			ft_add_token(&lexer->tokens, next_token);
			lexer->pos += len;
		}
		else
		{
			lexer->start = lexer->pos;
			while (lexer->src[lexer->pos] && !ft_isspace(lexer->src[lexer->pos])
				&& !ft_is_operator_char(lexer->src[lexer->pos]))
				lexer->pos++;
			lexer->word = ft_strndup(&lexer->src[lexer->start], lexer->pos - lexer->start);
			if (!lexer->word)
			{
				perror("ft_strndup | TODO: clean memory, refactor");
				exit(EXIT_FAILURE);
			}
			next_token = ft_new_token(WORD, lexer->word);
			if (!next_token)
			{
				perror("ft_new_token | TODO: clean memory, refactor");
				exit(EXIT_FAILURE);
			}
			ft_add_token(&lexer->tokens, next_token);
		}
	}
}
