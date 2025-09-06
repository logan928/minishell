/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:57:09 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/06 13:57:13 by uwettasi         ###   ########.fr       */
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

// int	exec_command_builtins(t_shell *shell, t_command *cmd)
// {
// 	pid_t	saved;

// 	saved = dup(STDOUT_FILENO);
// 	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, MAIN_SHELL))
// 		return (1);
// 	run_builtin(shell, cmd, MAIN_SHELL);
// 	dup2(saved, STDOUT_FILENO);
// 	close(saved);
// 	return (shell->exit_status);
// }

static void	exec_command_in_child(t_shell *shell, t_command *cmd)
{
	int		access_err;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, CHILD_SHELL))
		exit (1);
	if (cmd->args)
	{
		access_err = ft_check_access(shell, cmd);
		if (access_err)
			exit (access_err);
	}
	if (shell->parse_err == 5)
		exit (1);
	if (!cmd->args)
		exit(0);
	execve((cmd->args)[0], cmd->args, shell->env->data);
	perror("execve");
	exit(127);
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
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_trap);
	exitcode = exec_command_signal_handle(&status);
	if (exitcode != -1)
		return (exitcode);
	return (1);
}
