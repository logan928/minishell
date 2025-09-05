/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:36:40 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:38:59 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isoperator(t_token_kind kind)
{
	return (kind == PIPE || kind == OR_IF || kind == AND_IF || kind == NL);
}

int	ft_isredirection(t_token_kind kind)
{
	return (kind == LESS || kind == DLESS || kind == GREAT || kind == DGREAT);
}

int	ft_syntax_error(t_shell *shell, char *token)
{
	char	*err;

	err = fts_strjoin3cpy(shell, EUNEXPTKN, token, "'\n");
	fts_write(shell, err, STDERR_FILENO);
	shell->exit_status = 2;
	return (0);
}
