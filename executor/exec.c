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

int	exec_pipeline(t_shell *shell, t_ast *ast)
{
	int					count;
	t_ast				**commands;
	t_pipe_parameters	*tpp;

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
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_trap);
	if (tpp->sig)
		return (128 + tpp->sig);
	return (tpp->last_status);
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

// int	exec_ast(t_shell *shell, t_ast *ast)
// {
// 	if (!ast)
// 		return (0);
// 	if (ast -> type == AST_CMD)
// 		return (exec_command(shell, ast->cmd));
// 	else if (ast->type == AST_PIPE)
// 		return (exec_pipeline(shell, ast));
// 	else if (ast->type == AST_AND)
// 		return (handle_and(shell, ast));
// 	else if (ast->type == AST_OR)
// 		return (handle_or(shell, ast));
// 	else if (ast->type == AST_SUBSHELL)
// 		return (exec_subshell(shell, ast));
// 	else
// 	{
// 		printf("Non-defined AST type %s \n", ast->cmd->args[0]);
// 		return (1);
// 	}
// }
