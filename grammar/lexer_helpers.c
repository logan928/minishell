/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:32:53 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 16:39:33 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->word)
	{
		free(lexer->word);
		lexer->word = NULL;
	}
	if (lexer->tokens)
		ft_free_tokens(lexer->tokens);
	if (lexer->tmp == lexer->tmp2 && lexer->tmp)
		ft_free_tokens(lexer->tmp);
	else
	{
		if (lexer->tmp)
			ft_free_tokens(lexer->tmp);
		if (lexer->tmp2)
			ft_free_tokens(lexer->tmp2);
	}
	lexer->tokens = NULL;
	lexer->tmp = NULL;
	lexer->tmp2 = NULL;
}

int	ft_is_operator_char(char c)
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

int	ft_is_normal_char(char c)
{
	return (!ft_isspace(c) && !ft_is_operator_char(c));
}

t_token_kind	ft_get_token_kind(const char *s)
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

int	ft_get_operator_length(t_token_kind kind)
{
	if (kind == OR_IF || kind == AND_IF || kind == DLESS || kind == DGREAT)
		return (2);
	return (1);
}
