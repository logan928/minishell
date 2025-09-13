/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 14:58:41 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/10 17:25:58 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_command_child(t_shell *shell, t_pipe_parameters *tpp, \
			t_ast **commands, pid_t *pids)
{
	t_command	*cmd;

	cmd = commands[tpp->temp_counter]->cmd;
	if (cmd->args)
	{
		ft_skip_empty_vars(shell, cmd->args);
		if (!cmd->args[0])
			ft_critical_with_code(shell, 0, commands, pids);
		handle_expansion_cmd_child(shell, cmd);
	}
	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, CHILD_SHELL))
	{
		free_tpp(tpp, tpp->count - 1);
		ft_critical_with_code(shell, shell->exit_status, commands, pids);
	}
	if (cmd->command_kind == BUILTIN)
	{
		free_tpp(tpp, tpp->count - 1);
		run_builtin(shell, cmd, CHILD_SHELL, -1); 
		ft_critical_with_code(shell, shell->exit_status, commands, pids);
	}
	handle_check_access(shell, cmd, commands, pids);
	execve(cmd->path, cmd->args, shell->env->data);
	perror("execve");
	ft_critical_with_code(shell, 127, commands, pids);
}

static void	exec_command_child_wrapper(t_shell *shell, t_ast ***commands, \
			t_pipe_parameters *tpp, pid_t *pids)
{
	int	k;

	k = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (tpp->temp_counter > 0)
		dup2((tpp->pipefd)[tpp->temp_counter - 1][0], STDIN_FILENO);
	if (tpp->temp_counter < tpp->count - 1)
		dup2((tpp->pipefd)[tpp->temp_counter][1], STDOUT_FILENO);
	while (k < tpp->count - 1)
	{
		close((tpp->pipefd)[k][0]);
		close((tpp->pipefd)[k][1]);
		k++;
	}
	if ((*commands)[tpp->temp_counter]->type == AST_CMD \
			&& (*commands)[tpp->temp_counter]->cmd)
		exec_command_child(shell, tpp, *commands, pids);
	free_tpp(tpp, tpp->count - 1);
	ft_critical_with_code(shell, exec_ast(\
	shell, (*commands)[tpp->temp_counter]), *commands, pids);
}

static int	create_pipe_forks(t_shell *shell, t_ast ***commands, \
			pid_t *pids, t_pipe_parameters *tpp)
{
	int		j;

	j = 0;
	tpp->temp_counter = j;
	while (tpp->temp_counter < tpp->count)
	{
		pids[tpp->temp_counter] = fork();
		if (pids[tpp->temp_counter] == -1)
			return (perror("fork"), 1);
		if (pids[tpp->temp_counter] == 0) 
			exec_command_child_wrapper(shell, commands, tpp, pids);
		tpp->temp_counter++;
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

	pids = get_pid_ts(shell, tpp);
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
	free(pids);
	return (0);
}
