/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_ex_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:39:05 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/06 15:39:19 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_command_expansions(t_shell *shell, t_command *cmd)
{
	ft_variable_expansion(shell, cmd->args, 0);
	ft_field_splitting(shell, &cmd->args, 0);
	ft_file_exp(shell, &cmd->args, 1, 1);
	ft_quote_removal(shell, cmd->args, 0);
}

static void	exec_command_in_child(t_shell *shell, t_command *cmd)
{
	int		access_err;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, CHILD_SHELL))
		ft_critical_with_code(shell, 1, NULL, NULL);
	if (cmd->args)
	{
		access_err = ft_check_access(shell, cmd);
		if (access_err)
			ft_critical_with_code(shell, access_err, NULL, NULL);
	}
	if (shell->parse_err == 5)
		ft_critical_with_code(shell, 1, NULL, NULL);
	if (!cmd->args)
		ft_critical_with_code(shell, 0, NULL, NULL);
	execve(cmd->path, cmd->args, shell->env->data);
	perror("execve");
	ft_critical_with_code(shell, 127, NULL, NULL);
}

static int	exec_command_signal_handle(int *status)
{
	int	sig;

	sig = -1;
	if (WIFSIGNALED(*status))
	{
		sig = WTERMSIG(*status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		return (128 + sig);
	}
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	return (sig);
}

int	exec_command_externals(t_shell *shell, t_command *cmd)
{
	pid_t			pid;
	int				status;
	int				exitcode;

	pid = -1;
	status = -1;
	exitcode = -1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		exec_command_in_child(shell, cmd);
	waitpid(pid, &status, 0);
	ft_set_signals_main_pre();
	exitcode = exec_command_signal_handle(&status);
	if (exitcode != -1)
		return (exitcode);
	return (1);
}
