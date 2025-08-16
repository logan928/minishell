/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:19:02 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/15 11:54:08 by mkugan           ###   ########.fr       */
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
	l->quote = 0;
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

size_t ft_str_size(char *s)
{
	if (s)
		return (ft_strlen(s));
	else
		return 0;
}

char	*ft_str_join_free(char *s1, char *s2)
{
	size_t	size1;
	size_t	size2;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	size1 = ft_str_size(s1);
	size2 = ft_str_size(s2);
	res = malloc(size1 + size2 + 1);
	if (!res)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	if (s1)
		ft_memcpy(res, s1, size1);
	if (s2)
		ft_memcpy(res + size1, s2, size2);
	res[size1 + size2] = '\0';
	free(s1);
	free(s2);
	return (res);
}


/*
	* ft_expand processes each word segment by segment
	* A segment can be one of three types:
	* 	- Unquoted: $ is expanded;
	* 	- Single-quoted: every character is treated literally
	* 	- Double-quoted: $ is expanded
	* #? is expanded to the value of shell->exit_code
	* Single quotes are preserved inside double quotes, and vice versa
	* Outer quotes are removed during this stage (???)
*/
void	ft_expand(t_shell *shell)
{
	t_token	*t;
	char	*res;
	t_lexer *l;

	t = shell->lexer->tokens;
	l = shell->lexer;
	while (t)
	{
		if (t->token_kind == WORD)
		{
			res = ft_strdup("");
			l->pos = 0;
			l->quote = 0;
			l->start = -1;
			while (t->data[l->pos])
			{
				if (ft_isquote(t->data[l->pos]) && l->quote == 0)
					l->quote = t->data[(l->pos)++];
				else if (ft_isquote(t->data[l->pos]) && l->quote == t->data[l->pos])
				{
					l->quote = 0;
					l->pos++;
				}
				else if ((l->quote != '\'') && t->data[l->pos] == '$'
					&& ft_valid_env_char(t->data[l->pos + 1]))
				{
					l->start = ++(l->pos);
					while (t->data[l->pos] && ft_valid_env_char(t->data[l->pos]))
						(l->pos)++;
					res = ft_str_join_free(res, ft_get_env_var(&t->data[l->start], l->pos - l->start, shell->env));
				}
				else if ((l->quote != '\'') && t->data[l->pos] == '$'
					&& t->data[l->pos + 1] == '?')
				{
					res = ft_str_join_free(res, ft_itoa((long) shell->exit_status));
					l->pos += 2;
				}
				else
				{
					l->start = (l->pos)++;
					while (t->data[l->pos] && !ft_isquote(t->data[l->pos]) && t->data[l->pos] != '$')
						l->pos++;
					res = ft_str_join_free(res, ft_strndup(&t->data[l->start], l->pos - l->start));
				}
			}
			free(t->data);
			t->data = res;
		}
		t = t->next;
	}
}
