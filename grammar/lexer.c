/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:19:02 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/11 15:40:13 by mkugan           ###   ########.fr       */
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

void	ft_tokenize_op(t_lexer *l)
{
	t_token			*next_token;
	t_token_kind	kind;
	size_t			len;
	char			*op;
	
	kind = ft_get_token_kind(&l->src[l->pos]);
	len = ft_get_operator_length(kind);
	op = ft_strndup(&l->src[l->pos], len);
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
	ft_add_token(&l->tokens, next_token);
	l->pos += len;
}

void	ft_tokenize_nl(t_lexer *l)
{
	t_token			*next_token;
	char			*op;

	op = ft_strdup("newline");
	if (!op)
	{
		perror("ft_strndup | TODO: clean memory, refactor");
		exit(EXIT_FAILURE);
	}
	next_token = ft_new_token(NL, op);
	if (!next_token)
	{
		perror("ft_new_token | TODO: clean memory, refactor");
		exit(EXIT_FAILURE);
	}
	ft_add_token(&l->tokens, next_token);
}

void	ft_tokenize_word(t_lexer *l)
{
	t_token			*next_token;
	
	l->word = ft_strndup(&l->src[l->start], l->pos - l->start);
	if (!l->word)
	{
		perror("ft_strndup | TODO: clean memory, refactor");
		exit(EXIT_FAILURE);
	}
	next_token = ft_new_token(WORD, l->word);
	if (!next_token)
	{
		perror("ft_new_token | TODO: clean memory, refactor");
		exit(EXIT_FAILURE);
	}
	ft_add_token(&l->tokens, next_token);
	l->quote = 0;
}

void	lex(t_lexer *lexer)
{

	while (lexer->src[lexer->pos])
	{
		while (ft_isspace(lexer->src[lexer->pos]) && lexer->quote == 0)
			lexer->pos++;
		if (ft_is_operator_char(lexer->src[lexer->pos]) && lexer->quote == 0)
			ft_tokenize_op(lexer);
		else if (lexer->src[lexer->pos])
		{
			lexer->start = lexer->pos;
			while (lexer->src[lexer->pos]
				&& ((ft_isspace(lexer->src[lexer->pos]) && lexer->quote)
				|| (ft_is_operator_char(lexer->src[lexer->pos]) && lexer->quote)
				|| (!ft_isspace(lexer->src[lexer->pos]) && !ft_is_operator_char(lexer->src[lexer->pos]))))
			{
				if (ft_isquote(lexer->src[lexer->pos]) && !lexer->quote)
					lexer->quote = lexer->src[lexer->pos];
				else if (ft_isquote(lexer->src[lexer->pos]) && lexer->quote)
					lexer->quote = 0;
				lexer->pos++;
			}
			ft_tokenize_word(lexer);
		}
	}
	ft_tokenize_nl(lexer);
}

void	ft_expand(t_lexer *l)
{
	t_token	*t;
	size_t	pos;
	char	quote;
	size_t	start;

	t = l->tokens;
	while (t)
	{
		if (t->token_kind == WORD)
		{
			printf("Word: %s\n", t->data);
			pos = 0;
			quote = 0;
			while (t->data[pos])
			{
				if (t->data[pos] && t->data[pos] == '$' && quote != '\'')
				{
					start = ++pos;
					while (ft_valid_env_char(t->data[pos]))
						pos++;
					printf("%s\n", ft_get_env_var(&t->data[start], pos - start + 1, l->env));
				}
				pos++;
			}
		}
		t = t->next;
	}
}
