/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:40:42 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/06 15:40:46 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	run_builtin(t_shell *shell, t_command *cmd, int shell_type, pid_t fd)
{
	if (ft_strcmp(cmd->args[0], "exit", 0) == 0)
		ft_exit(shell, cmd->args, shell_type, fd);
	if (ft_strcmp(cmd->args[0], "echo", 0) == 0)
		ft_echo(shell, cmd->args);
	if (ft_strcmp(cmd->args[0], "env", 0) == 0)
		ft_env(shell, cmd->args);
	if (ft_strcmp(cmd->args[0], "pwd", 0) == 0)
		ft_pwd(shell, cmd->args);
	if (ft_strcmp(cmd->args[0], "cd", 0) == 0)
		ft_cd(shell, cmd->args);
	if (ft_strcmp(cmd->args[0], "unset", 0) == 0)
		ft_unset(shell, cmd->args);
	if (ft_strcmp(cmd->args[0], "export", 0) == 0)
		ft_export(shell, cmd->args);
	return (0);
}

int	exec_command_builtins(t_shell *shell, t_command *cmd)
{
	pid_t	saved;

	saved = dup(STDOUT_FILENO);
	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, MAIN_SHELL))
	{
		close(saved);
		return (1);
	}
	run_builtin(shell, cmd, MAIN_SHELL, saved);
	dup2(saved, STDOUT_FILENO);
	close(saved);
	return (shell->exit_status);
}
