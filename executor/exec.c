/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:50:29 by uwettasi          #+#    #+#             */
/*   Updated: 2025/08/31 21:09:34 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

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

static int	open_file(t_redir *redir, int shell_type, int flags)
{
	int	fd;

	fd = open(redir->file[0], flags, 0644);
	if (fd < 0)
	{
		perror("open");
		if (shell_type == CHILD_SHELL)
			exit(1);
		else
			return (-1);
	}
	return (fd);
}

static int	handle_r_in(t_redir *redir, int shell_type, int flags, \
	t_command_kind kind)
{
	int	fd;

	fd = open_file(redir, shell_type, flags);
	if (fd < 0)
		return (1);
	if (kind != BUILTIN)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

static int	handle_r_out(t_redir *redir, int shell_type, int flags)
{
	int	fd;

	fd = open_file(redir, shell_type, flags);
	if (fd < 0)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_r_app(t_redir *redir, int shell_type, int flags)
{
	int	fd;

	fd = open_file(redir, shell_type, flags);
	if (fd < 0)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_r_heredoc(t_redir *redir, int shell_type, \
	t_command_kind kind)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1) 
	{
		perror("pipe");
		if (shell_type == CHILD_SHELL)
			exit(1); 
		else
			return (1);
	}
	if (kind != BUILTIN)
	{
		write(pipefd[1], redir->file[0], strlen(redir->file[0]));
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	return (0);
}

static int	handle_redir(t_redir *redir, int shell_type, \
	t_command_kind kind )
{
	if (redir->kind == R_IN)
		return (handle_r_in(redir, shell_type, O_RDONLY, kind));
	else if (redir->kind == R_OUT)
		return (handle_r_out(redir, shell_type, O_WRONLY | O_CREAT | O_TRUNC));
	else if (redir->kind == R_APP)
		return (handle_r_app(redir, shell_type, O_WRONLY | O_CREAT | O_APPEND));
	else if (redir->kind == R_HDOC)
		return (handle_r_heredoc(redir, shell_type, kind));
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
		ft_variable_expansion(shell, redir->file, 0);
		ft_filename_expansion(shell, &redir->file, 0, 0);
		ft_quote_removal(shell, redir->file, 0);
		if (redir->kind != R_HDOC
			&& (!redir->file || !redir->file[0] || redir->file[1] != NULL))
		{
			err = ft_str_join3_cpy_safe(shell, "minishell: ", \
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

static int write_safe_return_wrapper(t_shell *shell, t_command *cmd,
	t_cehck_access_msgs acc_para, int exit_condition)
{
	char			*cmd_name;

	cmd_name = ft_strdup_safe(shell, cmd->args[0]);
	if (acc_para.is_access_exists)
	{
		ft_write_safe(shell,
			ft_str_join3_cpy_safe(shell, "minishell: ", cmd->args[0], \
			acc_para.msg), STDERR_FILENO);
	}
	else
	{
		ft_write_safe(shell,
			ft_str_join3_cpy_safe(shell, cmd->args[0], \
			acc_para.msg, ""), STDERR_FILENO);
	}
	free(cmd_name);
	return (exit_condition);
}

static int	handle_access_exist(t_shell *shell, t_command *cmd, t_cehck_access_msgs acc_para)
{
	if (ft_strchr(cmd->args[0], '/') != NULL)
	{
		acc_para.msg = ": No such file or directory\n";
		acc_para.is_access_exists = true;
		return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_FOUND));
	}
	else
	{
		acc_para.msg = ": command not found\n";
		acc_para.is_access_exists = false;
		return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_FOUND));
	}
}

static int	ft_check_access(t_shell *shell, t_command *cmd)
{
	char				*cmd_name;
	t_cmd_access		access;
	t_cehck_access_msgs	acc_para;

	cmd_name = ft_strdup_safe(shell, cmd->args[0]);
	access = ft_get_cmd_path(shell, cmd->args);
	acc_para = (t_cehck_access_msgs){NULL, true};
	if (!access.exist)
	{
		/*
		if (ft_strchr(cmd->args[0], '/') != NULL)
		{
			acc_para.msg = ": No such file or directory\n";
			acc_para.is_access_exists = true;
			return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_FOUND));
		}
		else
		{
			acc_para.msg = ": command not found\n";
			acc_para.is_access_exists = false;
			return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_FOUND));
		}
			*/
		return (handle_access_exist(shell, cmd, acc_para));
	}
	else if (access.is_dir)
	{
		acc_para.msg = ": Is a directory\n";
		acc_para.is_access_exists = true;
		return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_EXEC));
	}
	else if (!access.executable)
	{
		acc_para.msg = ": Permission denied\n";
		acc_para.is_access_exists = true;
		return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_EXEC));
	}
	free(cmd_name);
	return (0);
}

static	int	exec_command(t_shell *shell, t_command *cmd)
{
	pid_t			pid;
	int				status;
	int				access_err;

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

	if (apply_redirs(shell, cmd->redirs, cmd->command_kind, CHILD_SHELL))
		exit(shell->exit_status);
	if (cmd->command_kind == BUILTIN) // builtins in child
	{
		exit(run_builtin(shell, cmd, CHILD_SHELL)); // run in child, not parent
	}

	access_err = ft_check_access(shell, cmd);
	if (access_err)
		exit(access_err);
	
	execve((cmd->args)[0], cmd->args, shell->env->data); //use resolved path
	perror("execve");
	exit(127);
}

static int	exec_pipeline(t_shell *shell, t_ast *ast)
{
	int		count;
	t_ast	*node;
	int		i;
	int		j;

	count = 0;
	node = ast;
	i = 0;
	j = 0;
	while (node && node->type == AST_PIPE)
	{
		count++;
		node = node->left;
	}
	count++; 
	node = ast;
	t_ast	*commands[count];
	i = count-1;//get rid
	while (node && node->type == AST_PIPE)
	{
		commands[i] = node->right;
		node = node->left;
		i--;
	}
	commands[i] = node; 

	int pipefd[count - 1][2];
	while ( j < count - 1)
	{
		if (pipe(pipefd[j]) == -1)
		{
			perror("pipe");
			return (1);
		}
		j++;
	}
	j = 0; //counter reset to save lines

	pid_t pids[count];
	while (j < count)
	{
		int k = 0;
		pids[j] = fork();
		if (pids[j] == -1)
		{
			perror("fork");
			return (1);
		}
		if (pids[j] == 0) // this is the child process. 
		{

			if (j > 0)
			{
				dup2(pipefd[j-1][0], STDIN_FILENO);
			}

			if (j < count - 1)
			{
				dup2(pipefd[j][1], STDOUT_FILENO);
			}

			// close all pipe fds in child
			while ( k < count - 1)
			{
				close(pipefd[k][0]);
				close(pipefd[k][1]);
				k++;
			}

			if (commands[j]->type == AST_CMD && commands[j]->cmd)
				exec_command_child(shell, commands[j]->cmd); // builtins inside pipe needs to be executed inside child

			exit(exec_ast(shell, commands[j])); // simple command
		}
		j++;
	}

	j = 0;
	while ( j < count - 1)
	{
		close(pipefd[j][0]);
		close(pipefd[j][1]);
		j++;
	}

	// wait for all children
	int status = 0;
	int  last_status = 0;
	j =0;
	while  (j < count)
	{
		if (waitpid(pids[j], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
		j++;
	}
	return (last_status);
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
