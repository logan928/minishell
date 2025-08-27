#include "../minishell.h"
#include <sys/wait.h>
#include <fcntl.h>

int run_builtin(t_shell *shell, t_command *cmd)
{
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		ft_exit(shell, cmd->args);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		ft_echo(shell, cmd->args);
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		ft_env(shell, cmd->args);
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		ft_pwd(shell, cmd->args);
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		ft_cd(shell, cmd->args);
	if (ft_strncmp(cmd->args[0], "unset", 5) == 0)
		ft_unset(shell, cmd->args);
	return 0;
}


static void apply_redirs(t_redir *redir)
{
	printf("inside applying redirections");
	while (redir)
	{
		int fd;

		fd = -1;
		if (redir->kind == R_IN)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd < 0) 
			{ 
				perror("open"); 
				exit(1); 
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->kind == R_OUT)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0) 
			{ 
				perror("open"); 
				exit(1); 
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->kind == R_APP)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0) 
			{ 
				perror("open"); 
				exit(1);
			 }
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->kind == R_HDOC)
		{
			printf("heredoc processing...\n");//																															
		}
		redir = redir->next;
	}
}


static int exec_command(t_shell *shell, t_command *cmd)
{
	if(cmd->command_kind == BUILTIN)
		return(run_builtin(shell, cmd));
	else
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			apply_redirs(cmd->redirs);
			t_cmd_access access = ft_get_cmd_path(shell, cmd->args);
			
			if (access.executable)
			{
				execve((cmd->args)[0], cmd->args, cmd->env);
				perror("execve");
				exit(127);
			}
			
			
		}
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return WEXITSTATUS(status);
		return 1;
	}
	
}

static int exec_pipeline(t_shell *shell, t_ast *ast)
{
	//(void) ast;
	printf("creating pipelines\n");
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return 1;
	}



	pid_t left_pid = fork();
	if (left_pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		if (ast->left->type == AST_CMD && ast->left->cmd)
			apply_redirs(ast->left->cmd->redirs);
		exit(exec_ast(shell, ast->left));
	}

	pid_t right_pid = fork();
	if (right_pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		if (ast->right->type == AST_CMD && ast->right->cmd)
			apply_redirs(ast->right->cmd->redirs);
		exit(exec_ast(shell, ast->right));
	}

	close(pipefd[0]);
	close(pipefd[1]);


	int status;
	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);


	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return 1;
}

static int exec_subshell(t_ast *ast)
{
	(void) ast;
	printf("inside sub-shell");
	return 0;
}

int exec_ast(t_shell *shell, t_ast *ast)
{
	if (!ast)
		return 0;

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
		return left;
	}
	else if (ast->type == AST_OR)
	{
		int left;

		left = exec_ast(shell, ast->left);
		if (left != 0)
			return(exec_ast(shell, ast->right));
		return left;
	}
	else if (ast->type == AST_SUBSHELL)
		return (exec_subshell(ast));
	else
	{
		printf("Non-defined AST type \n");
		return 1;
	}

}
