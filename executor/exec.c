/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:50:29 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/04 11:14:35 by mkugan           ###   ########.fr       */
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
		tmp = ft_strdup_safe(shell, redir->file[0]);
		ft_skip_empty_vars(shell, redir->file);
		ft_variable_expansion(shell, redir->file, 0);
		ft_file_exp(shell, &redir->file, 0, 0);
		ft_quote_removal(shell, redir->file, 0);
		if (redir->kind != R_HDOC
			&& (!redir->file || !redir->file[0] || redir->file[1] != NULL))
		{
			err = fts_strjoin3cpy(shell, "minishell: ", \
				tmp, ": ambiguous redirect\n");
			ft_write_safe(shell, err, STDERR_FILENO);
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



static	int	exec_command(t_shell *shell, t_command *cmd)
{
	pid_t			pid;
	int				status;
	int				access_err;

	if (cmd->args)
	{
		ft_skip_empty_vars(shell, cmd->args);
		if (!cmd->args[0])
			return (0);
		ft_variable_expansion(shell, cmd->args, 0);
		ft_field_splitting(shell, &cmd->args, 0);
		ft_file_exp(shell, &cmd->args, 1, 1);
		ft_quote_removal(shell, cmd->args, 0);
	}
	pid = -1;
	status = -1;
	if (cmd->command_kind == BUILTIN)
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
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		pid = fork();
		if (pid == 0)
		{
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
		waitpid(pid, &status, 0);
		signal(SIGINT, ft_sigint_handler);
		signal(SIGQUIT, ft_sigquit_trap);
		if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (sig == SIGQUIT)
				write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			return 128 + sig;
		}
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
}

static int	exec_command_child(t_shell *shell, t_command *cmd)
{
	int	access_err;

	if (cmd->args)
	{
		ft_skip_empty_vars(shell, cmd->args);
		if (!cmd->args[0])
			exit(0);
		ft_variable_expansion(shell, cmd->args, 0);
		ft_field_splitting(shell, &cmd->args, 0);
		ft_file_exp(shell, &cmd->args, 1, 1);
		ft_quote_removal(shell, cmd->args, 0);
	}
	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, CHILD_SHELL))
		exit(shell->exit_status);
	if (cmd->command_kind == BUILTIN) // builtins in child
	{
		run_builtin(shell, cmd, CHILD_SHELL); // run in child, not parent
		exit(shell->exit_status);
	}

	access_err = ft_check_access(shell, cmd);
	if (access_err)
		exit(access_err);
	
	execve((cmd->args)[0], cmd->args, shell->env->data); //use resolved path
	perror("execve");
	exit(127);
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

	while (node && node->type == AST_PIPE)
	{
		commands[i] = node->right;
		node = node->left;
		i--;
	}
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

static int	create_pipe_forks(t_shell *shell, t_ast ***commands, pid_t *pids, int ***pipefd, t_pipe_parameters *tpp)
{
	int		k;
	int		j;

	j = 0;
	while (j < tpp->count)
	{
		k = 0;
		pids[j] = fork();
		if (pids[j] == -1)
			return (perror("fork"), 1);
		if (pids[j] == 0) 
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (j > 0)
				dup2((*pipefd)[j-1][0], STDIN_FILENO);
			if (j < tpp->count - 1)		
				dup2((*pipefd)[j][1], STDOUT_FILENO);
			while ( k < tpp->count - 1)
			{
				close((*pipefd)[k][0]);
				close((*pipefd)[k][1]);
				k++;
			}
			if ((*commands)[j]->type == AST_CMD && (*commands)[j]->cmd)
				exec_command_child(shell, (*commands)[j]->cmd);
			exit(exec_ast(shell, (*commands)[j]));
		}
		j++;
	}
	return (0);
}

static int	exec_pipeline_core (t_shell *shell, int ***pipefd, t_ast ***commands, t_pipe_parameters *tpp)
{
	pid_t	pids[(tpp)->count];
	//int		k;
	int		j;

	// j = 0;
	// while (j < tpp->count)
	// {
	// 	k = 0;
	// 	pids[j] = fork();
	// 	if (pids[j] == -1)
	// 		return (perror("fork"), 1);
	// 	if (pids[j] == 0) 
	// 	{
	// 		signal(SIGINT, SIG_DFL);
	// 		signal(SIGQUIT, SIG_DFL);
	// 		if (j > 0)
	// 			dup2((*pipefd)[j-1][0], STDIN_FILENO);
	// 		if (j < tpp->count - 1)		
	// 			dup2((*pipefd)[j][1], STDOUT_FILENO);
	// 		while ( k < tpp->count - 1)
	// 		{
	// 			close((*pipefd)[k][0]);
	// 			close((*pipefd)[k][1]);
	// 			k++;
	// 		}
	// 		if ((*commands)[j]->type == AST_CMD && (*commands)[j]->cmd)
	// 			exec_command_child(shell, (*commands)[j]->cmd);
	// 		exit(exec_ast(shell, (*commands)[j]));
	// 	}
	// 	j++;
	// }

	if (create_pipe_forks(shell, commands, pids, pipefd, tpp))
		return (1);

	j = 0;
	while ( j < tpp->count - 1)
	{
		close((*pipefd)[j][0]);
		close((*pipefd)[j][1]);
		j++;
	}
	j =0;
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
	int					**pipefd;
	t_pipe_parameters	*tpp;

	tpp = malloc(sizeof(t_pipe_parameters));
	if (!tpp)
		return (1);
	*tpp = (t_pipe_parameters){0, 0, 0, false, false, 0};
	count = 0;
	pipefd = NULL;
	commands = NULL;
	if (get_fd_array(ast, &commands, &count, &pipefd))
		return (1);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	tpp->count = count;
	if (exec_pipeline_core(shell, &pipefd, &commands, tpp))
		return (1);
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_trap);
	if (tpp->sig)
		return (128 + tpp->sig);
	return (tpp->last_status);
}



static int	exec_subshell(t_shell *shell, t_ast *ast)
{
	//(void) ast;
	//printf("inside sub-shell\n");
	if (!ast || !ast->left) 
	{ 
		printf("no left\n");
		return (0);
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		// child executes the subshell AST
		// (redirections on the subshell node itself should apply here)
		if (ast->cmd && ast->cmd->redirs)
			apply_redirs(shell, ast->cmd->redirs, ast->cmd->command_kind, CHILD_SHELL);

		exit(exec_ast(shell, ast->left));
	}
	int status;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return (1);

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
	{
		int left;

		left = exec_ast(shell, ast->left);
		if(left == 0)
			return(exec_ast(shell, ast->right));
		return (left);
	}
	else if (ast->type == AST_OR)
	{
		int left;

		left = exec_ast(shell, ast->left);
		if (left != 0)
			return(exec_ast(shell, ast->right));
		return (left);
	}
	else if (ast->type == AST_SUBSHELL)
		return (exec_subshell(shell, ast));
	else
	{
		printf("Non-defined AST type %s \n", ast->cmd->args[0]);
		return (1);
	}

}
