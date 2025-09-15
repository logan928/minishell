/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:50:27 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:37:23 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_check_last_quote(t_shell *shell, t_token *t)
{
	char	q;
	size_t	i;

	q = '\0';
	i = 0;
	while (t->data[i])
	{
		if (!q && ft_isquote(t->data[i]))
			q = t->data[i];
		else if (q && ft_isquote(t->data[i]) && t->data[i] == q)
			q = '\0';
		i++;
	}
	if (q)
	{
		shell->exit_status = 2;
		if (q == '"')
			fts_write(shell, fts_strjoin3cpy(shell,
					UNEXPQOUT, "\"", "'\n"), STDERR_FILENO);
		else
			fts_write(shell, fts_strjoin3cpy(shell,
					UNEXPQOUT, "'", "'\n"), STDERR_FILENO);
		return (0);
	}
	return (1);
}

static int	ft_check_next_token(t_token *c, t_token *n)
{
	if (ft_isoperator(c->token_kind) && n && ft_isoperator(n->token_kind))
		return (1);
	else if (ft_isredirection(c->token_kind) && n->token_kind != WORD)
		return (1);
	if (c->token_kind == L_PAREN && n->token_kind == R_PAREN)
		return (1);
	if (c->token_kind == R_PAREN && n->token_kind == L_PAREN)
		return (1);
	if (c->token_kind == R_PAREN && n->token_kind == WORD)
		return (1);
	return (0);
}

static int	ft_check_parens(t_shell *shell)
{
	t_token			*token;
	t_token			*next;
	int				p_depth;

	p_depth = 0;
	token = shell->lexer->tokens;
	while (token && token->token_kind != NL)
	{
		next = token->next;
		if (token->token_kind == L_PAREN)
			p_depth++;
		else if (token->token_kind == R_PAREN)
		{
			if (p_depth == 0)
				return (ft_syntax_error(shell, token->data));
			p_depth--;
		}
		token = next;
	}
	if (p_depth > 0)
		return (ft_syntax_error(shell, token->data));
	return (1);
}

int	ft_check_syntax(t_shell *shell)
{
	t_token	*token;
	t_token	*next;
	int		i;
	t_token	*last_word;

	i = 0;
	token = shell->lexer->tokens;
	if (token->token_kind == NL)
		return (0);
	while (token && token->token_kind != NL)
	{
		if (token->token_kind == WORD)
			last_word = token;
		next = token->next;
		if (i == 0 && ft_isoperator(token->token_kind))
			return (ft_syntax_error(shell, token->data));
		if (ft_check_next_token(token, next))
			return (ft_syntax_error(shell, next->data));
		i++;
		token = next;
	}
	if (!ft_check_parens(shell))
		return (0);
	return (ft_check_last_quote(shell, last_word));
}
