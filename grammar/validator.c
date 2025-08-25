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

	err = ft_str_join3_cpy_safe(shell, EUNEXPTKN, token, "'\n");
	ft_write_safe(shell, err, STDERR_FILENO);
	return (0);
}

int	ft_check_syntax(t_shell *shell)
{
	t_token	*token;
	t_token	*next;
	int		i;

	i = 0;
	token = shell->lexer->tokens;
	while (token && token->token_kind != NL)
	{
		next = token->next;
		if ((i == 0 && ft_isoperator(token->token_kind))
			|| (ft_isoperator(token->token_kind) && next && ft_isoperator(next->token_kind)))
			return (ft_syntax_error(shell, next->data));
		if (ft_isredirection(token->token_kind) && next->token_kind != WORD)
			return (ft_syntax_error(shell, next->data));
		i++;
		token = next;
	}
	return (1);
}
