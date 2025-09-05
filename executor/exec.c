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

int	run_builtin(t_shell *shell, t_command *cmd, int shell_type)
{
	if (ft_strcmp(cmd->args[0], "exit", 0) == 0)
		ft_exit(shell, cmd->args, shell_type);
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

static	int	apply_redirs(t_shell *shell, t_redir *redir, \
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
			return (1);
		}
		if (handle_redir(redir, shell_type, kind))
			return (1);
		redir = redir->next;
	}
	return (0);
}

static void exec_command_expansions(t_shell *shell, t_command *cmd)
{
	ft_variable_expansion(shell, cmd->args, 0);
	ft_field_splitting(shell, &cmd->args, 0);
	ft_file_exp(shell, &cmd->args, 1, 1);
	ft_quote_removal(shell, cmd->args, 0);	
}

static int exec_command_builtins(t_shell *shell, t_command *cmd)
{
	pid_t saved;

	saved = dup(STDOUT_FILENO);
	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, MAIN_SHELL))
		return (1);
	run_builtin(shell, cmd, MAIN_SHELL);
	dup2(saved, STDOUT_FILENO);
	close(saved);
	return (shell->exit_status);
}

static void exec_command_in_child(t_shell *shell, t_command *cmd)
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

static int exec_command_signal_handle (int *status)
{
	int sig;

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

static int	exec_command_externals(t_shell *shell, t_command *cmd)
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
	
	// if (WIFSIGNALED(status))
	// {
	// 	int sig = WTERMSIG(status);
	// 	if (sig == SIGINT)
	// 		write(STDOUT_FILENO, "\n", 1);
	// 	else if (sig == SIGQUIT)
	// 		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	// 	return (128 + sig);
	// }
	// if (WIFEXITED(status))
	// 	return (WEXITSTATUS(status));
	exitcode = exec_command_signal_handle(&status);
	if (exitcode != -1)
		return (exitcode);
	return (1);
}

static	int	exec_command(t_shell *shell, t_command *cmd)
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

static void	handle_expansion_cmd_child(t_shell *shell, t_command *cmd)
{
	ft_variable_expansion(shell, cmd->args, 0);
	ft_field_splitting(shell, &cmd->args, 0);
	ft_file_exp(shell, &cmd->args, 1, 1);
	ft_quote_removal(shell, cmd->args, 0);
}

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

static void	search_commands(t_ast **node, t_ast **commands, int *i)
{
	while (*node && (*node)->type == AST_PIPE)
	{
		commands[*i] = (*node)->right;
		(*node) = (*node)->left;
		(*i)--;
	}
}

static t_ast	**get_leaf_commmands( t_ast *n, int *pipe_count)
{
	t_ast	*node;
	int		i;
	int		count;
	t_ast	**commands;

	node = n;
	count = 0;
	while (node && node->type == AST_PIPE)
	{
		count++;
		node = node->left;
	}
	count++; 
	node = n;
	commands = malloc(sizeof(t_ast *) * count);
	if (!commands)
		return (NULL);
	i = count-1;
	search_commands(&node, commands, &i);
	commands[i] = node;
	*pipe_count = count;
	return (commands);
}

static int	get_fd_array(t_ast *ast, t_ast ***commands, int *count, int ***pipefd)
{
	int	j;
	int	pipe_count;

	j = 0;
	pipe_count = 0;
	*commands = get_leaf_commmands(ast, &pipe_count);
	if (!*commands)
		return (1);
	*count = pipe_count;
	*pipefd = malloc(sizeof(int *) * (pipe_count - 1));
	if (!*pipefd)
		return (1);
	while ( j < pipe_count - 1)
	{
		(*pipefd)[j] = malloc(sizeof(int) * 2);
		if (!(*pipefd)[j])
			return (1);
		if (pipe((*pipefd)[j]) == -1)
			return (perror("pipe"), 1);
		j++;
	}
	return (0);
}

static void	exec_command_child_wrapper(t_shell *shell, t_ast ***commands, t_pipe_parameters *tpp, int j)
{
	int	k;

	k = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (j > 0)
		dup2((tpp->pipefd)[j-1][0], STDIN_FILENO);
	if (j < tpp->count - 1)		
		dup2((tpp->pipefd)[j][1], STDOUT_FILENO);
	while ( k < tpp->count - 1)
	{
		close((tpp->pipefd)[k][0]);
		close((tpp->pipefd)[k][1]);
		k++;
	}
	if ((*commands)[j]->type == AST_CMD && (*commands)[j]->cmd)
		exec_command_child(shell, (*commands)[j]->cmd);
	exit(exec_ast(shell, (*commands)[j]));
}

static int	create_pipe_forks(t_shell *shell, t_ast ***commands, pid_t *pids, t_pipe_parameters *tpp)
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
	while  (j < tpp->count)
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

static int	exec_pipeline_core (t_shell *shell, int ***pipefd, t_ast ***commands, t_pipe_parameters *tpp)
{
	pid_t	pids[(tpp)->count];
	int		j;

	if (create_pipe_forks(shell, commands, pids, tpp))
		return (1);
	j = 0;
	while ( j < tpp->count - 1)
	{
		close((*pipefd)[j][0]);
		close((*pipefd)[j][1]);
		j++;
	}
	j =0;
	wait_child_exit(tpp, pids);
	if (tpp->new_line)
		write(STDOUT_FILENO, "\n", 1);
	else if (tpp->core_dump)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	return (0);
}

static int	exec_pipeline(t_shell *shell, t_ast *ast)
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
	if (exec_pipeline_core(shell,  &tpp->pipefd, &commands, tpp))
		return (1);
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_trap);
	if (tpp->sig)
		return (128 + tpp->sig);
	return (tpp->last_status);
}

static int	exec_subshell(t_shell *shell, t_ast *ast)
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
			apply_redirs(shell, ast->cmd->redirs, ast->cmd->command_kind, CHILD_SHELL);
		exit(exec_ast(shell, ast->left));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return (1);
}

static int	handle_and(t_shell *shell, t_ast *ast)
{
	int	left;

	left = exec_ast(shell, ast->left);
	if(left == 0)
		return(exec_ast(shell, ast->right));
	return (left);
}

static int	handle_or(t_shell *shell, t_ast *ast)
{
	int	left;

	left = exec_ast(shell, ast->left);
	if(left != 0)
		return(exec_ast(shell, ast->right));
	return (left);
}

int	exec_ast(t_shell *shell, t_ast *ast)
{
	if (!ast)
		return (0);
	if (ast -> type == AST_CMD)
		return (exec_command(shell, ast->cmd));
	else if (ast->type == AST_PIPE)
		return (exec_pipeline(shell, ast));
	else if (ast->type == AST_AND)
		return (handle_and(shell, ast));
	else if (ast->type == AST_OR)
		return (handle_or(shell, ast));
	else if (ast->type == AST_SUBSHELL)
		return (exec_subshell(shell, ast));
	else
	{
		printf("Non-defined AST type %s \n", ast->cmd->args[0]);
		return (1);
	}
}
