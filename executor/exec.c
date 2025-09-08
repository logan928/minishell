/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:50:29 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/05 15:04:08 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

int	exec_command(t_shell *shell, t_command *cmd)
{
	if (cmd->args)
	{
		ft_skip_empty_vars(shell, cmd->args);
		if (!cmd->args[0])
			return (0);
		exec_command_expansions(shell, cmd);
	}
	if (cmd->command_kind == BUILTIN)
		return (exec_command_builtins(shell, cmd));
	else
		return (exec_command_externals(shell, cmd));
}

// static void	free_commands(t_ast **commands, int count)
// {
// 	int	i;

// 	if (!commands)
// 		return;
// 	i = 0;
// 	while (i < (count) && commands[i])
// 	{
// 		printf("%d", i);
// 		free_ast(commands[i]);
// 		i++;
// 	}
// 	free(commands);
// }

int	exec_pipeline(t_shell *shell, t_ast *ast)
{
	int					count;
	t_ast				**commands;
	t_pipe_parameters	*tpp;
	int					last_s;

	tpp = malloc(sizeof(t_pipe_parameters));
	if (!tpp)
		return (1);
	*tpp = (t_pipe_parameters){0, 0, 0, false, false, 0, NULL};
	count = 0;
	commands = NULL;
	if (get_fd_array(ast, &commands, &count, &tpp->pipefd))
		return (1);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	tpp->count = count;
	if (exec_pipeline_core(shell, &tpp->pipefd, &commands, tpp))
		return (1);
	//printf("%d\n", tpp->count);
	// free_commands(commands, tpp->count);//todo
	
	ft_set_signals_main_pre();
	if (tpp->sig)	
		return (128 + tpp->sig);
	last_s = tpp->last_status;
	free(tpp);
	return (last_s);
}

int	exec_subshell(t_shell *shell, t_ast *ast)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (!ast || !ast->left) 
		return (printf("no left\n"), 0);
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		if (ast->cmd && ast->cmd->redirs)
			apply_redirs(shell, ast->cmd->redirs, ast->cmd->command_kind, \
			CHILD_SHELL);
		exit(exec_ast(shell, ast->left));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	handle_and(t_shell *shell, t_ast *ast)
{
	int	left;

	left = exec_ast(shell, ast->left);
	if (left == 0)
		return (exec_ast(shell, ast->right));
	return (left);
}

int	handle_or(t_shell *shell, t_ast *ast)
{
	int	left;

	left = exec_ast(shell, ast->left);
	if (left != 0)
		return (exec_ast(shell, ast->right));
	return (left);
}
