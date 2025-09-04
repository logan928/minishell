/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:50:27 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/09 15:03:13 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

static int	ft_isoperator(t_token_kind kind)
{
	return (kind == PIPE || kind == OR_IF || kind == AND_IF || kind == NL);
}

static int	ft_isredirection(t_token_kind kind)
{
	return (kind == LESS || kind == DLESS || kind == GREAT || kind == DGREAT);
}

static int	ft_syntax_error(t_shell *shell, char *token)
{
	char	*err;

	err = fts_strjoin3cpy(shell, EUNEXPTKN, token, "'\n");
	ft_write_safe(shell, err, STDERR_FILENO);
	shell->exit_status = 2;
	return (0);
}

static int	ft_check_last_quote(t_shell *shell, t_token *t)
{
	char	q;
	size_t	i;
	
	q = '\0';
	i = 0;
	while(t->data[i])
	{
		if (!q && ft_isquote(t->data[i]))
			q = t->data[i];
		else if (q && ft_isquote(t->data[i]) == q)
			q = '\0';
		i++;
	}
	if (q)
	{
		shell->exit_status = 1;
		if (q == '"')
			ft_write_safe(shell, fts_strjoin3cpy(shell,
			"minishell: unexpected EOF while looking for matching`\"'\n", "", ""), STDERR_FILENO);
		else
			ft_write_safe(shell, fts_strjoin3cpy(shell,
			"minishell: unexpected EOF while looking for matching`\''\n", "", ""), STDERR_FILENO);
		return (0);
	}
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
		else if (ft_isoperator(token->token_kind) && next
			&& ft_isoperator(next->token_kind))
			return (ft_syntax_error(shell, next->data));
		if (ft_isredirection(token->token_kind) && next->token_kind != WORD)
			return (ft_syntax_error(shell, next->data));
		i++;
		token = next;
	}
	return (ft_check_last_quote(shell, last_word));
}
