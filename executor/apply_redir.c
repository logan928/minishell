/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:40:45 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/10 11:50:59 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	apply_redirs(t_shell *shell, t_redir *redir, \
	t_command_kind kind, int shell_type)
{
	char	*tmp;
	char	*err;

	while (redir)
	{
		tmp = fts_strdup(shell, redir->file[0]);
		ft_skip_empty_vars(shell, redir->file);
		ft_variable_expansion(shell, redir->file, 0);
		ft_file_exp(shell, &redir->file, 0, 0);
		ft_quote_removal(shell, redir->file, 0);
		if (redir->kind != R_HDOC
			&& (!redir->file || !redir->file[0] || redir->file[1] != NULL))
		{
			err = fts_strjoin3cpy(shell, "minishell: ", \
				tmp, ": ambiguous redirect\n");
			fts_write(shell, err, STDERR_FILENO);
			shell->exit_status = 1;
			shell->parse_err = 1;
			free(tmp);
			return (1);
		}
		free(tmp);
		if (handle_redir(shell, redir, shell_type, kind))
			return (1);
		redir = redir->next;
	}
	return (0);
}
