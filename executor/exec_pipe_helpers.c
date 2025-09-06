/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 14:58:41 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/06 14:58:44 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exec_command_child(t_shell *shell, t_command *cmd)
{
	int	access_err;

	if (cmd->args)
	{
		ft_skip_empty_vars(shell, cmd->args);
		if (!cmd->args[0])
			exit(0);
		handle_expansion_cmd_child(shell, cmd);
	}
	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, CHILD_SHELL))
		exit(shell->exit_status);
	if (cmd->command_kind == BUILTIN)
	{
		run_builtin(shell, cmd, CHILD_SHELL); 
		exit(shell->exit_status);
	}
	access_err = ft_check_access(shell, cmd);
	if (access_err)
		exit(access_err);
	execve((cmd->args)[0], cmd->args, shell->env->data);
	perror("execve");
	exit(127);
}

static void	exec_command_child_wrapper(t_shell *shell, t_ast ***commands, \
			t_pipe_parameters *tpp, int j)
{
	int	k;

	k = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (j > 0)
		dup2((tpp->pipefd)[j - 1][0], STDIN_FILENO);
	if (j < tpp->count - 1)
		dup2((tpp->pipefd)[j][1], STDOUT_FILENO);
	while (k < tpp->count - 1)
	{
		close((tpp->pipefd)[k][0]);
		close((tpp->pipefd)[k][1]);
		k++;
	}
	if ((*commands)[j]->type == AST_CMD && (*commands)[j]->cmd)
		exec_command_child(shell, (*commands)[j]->cmd);
	exit(exec_ast(shell, (*commands)[j]));
}

static int	create_pipe_forks(t_shell *shell, t_ast ***commands, \
			pid_t *pids, t_pipe_parameters *tpp)
{
	int		j;

	j = 0;
	while (j < tpp->count)
	{
		pids[j] = fork();
		if (pids[j] == -1)
			return (perror("fork"), 1);
		if (pids[j] == 0) 
			exec_command_child_wrapper(shell, commands, tpp, j);
		j++;
	}
	return (0);
}

static void	wait_child_exit(t_pipe_parameters *tpp, pid_t *pids)
{
	int	j;

	j = 0;
	while (j < tpp->count)
	{
		if (waitpid(pids[j], &(tpp->status), 0) > 0)
		{
			tpp->sig = 0;
			if (WIFSIGNALED(tpp->status))
			{
				tpp->sig = WTERMSIG(tpp->status);
				if (tpp->sig == SIGINT)
					tpp->new_line = true;
				else if (tpp->sig == SIGQUIT)
					tpp->core_dump = true;
			}
			if (WIFEXITED(tpp->status))
				tpp->last_status = WEXITSTATUS(tpp->status);
		}
		j++;
	}
}

int	exec_pipeline_core(t_shell *shell, int ***pipefd, t_ast ***commands, \
	t_pipe_parameters *tpp)
{
	pid_t	*pids;
	int		j;

	// pids = malloc((tpp->count) * sizeof(pid_t));
	// if (pids == NULL) 
	// {
	// 	perror("malloc failed");
	// 	exit(EXIT_FAILURE);
	// }
	pids = get_pid_ts(tpp);
	if (create_pipe_forks(shell, commands, pids, tpp))
		return (1);
	j = 0;
	while (j < tpp->count - 1)
	{
		close((*pipefd)[j][0]);
		close((*pipefd)[j][1]);
		j++;
	}
	wait_child_exit(tpp, pids);
	if (tpp->new_line)
		write(STDOUT_FILENO, "\n", 1);
	else if (tpp->core_dump)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	return (0);
}
