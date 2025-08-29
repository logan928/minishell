#include "../minishell.h"
#include <sys/wait.h>
#include <fcntl.h>

int run_builtin(t_shell *shell, t_command *cmd, int shell_type)
{
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		ft_exit(shell, cmd->args, shell_type);
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


static void apply_redirs(t_shell *shell, t_redir *redir)
{
	//printf("inside applying redirections\n");
	(void)shell;
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
	{
		apply_redirs(shell, cmd->redirs);
		return(run_builtin(shell, cmd, MAIN_SHELL));
	}
	else
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			apply_redirs(shell, cmd->redirs);
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

/*
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
	
} */
static int exec_command_child(t_shell *shell, t_command *cmd) 
{
	apply_redirs(shell, cmd->redirs); // preserve redirections

	if (cmd->command_kind == BUILTIN) // builtins in a pipeline must run in forked child
	{
		exit(run_builtin(shell, cmd, CHILD_SHELL)); // changed: run in child, not parent
	}

	t_cmd_access access = ft_get_cmd_path(shell, cmd->args);
	if (access.executable)
	{
		execve((cmd->args)[0], cmd->args, cmd->env); // changed: use resolved path
		perror("execve");
	}
	exit(127);
}
static int exec_pipeline(t_shell *shell, t_ast *ast)
{
	int count = 0;
	t_ast *node = ast;
	int i = 0;
	int j = 0;

	while (node && node->type == AST_PIPE)
	{
		count++;
		node = node->left;
	}
	count++; 

	//printf("count: %d\n", count);

	t_ast *commands[count];
	node = ast;

	i = count-1;
	while (node && node->type == AST_PIPE)
	{
		//commands[i++] = node->left;
		//node = node->right;
		commands[i] = node->right;
		node = node->left;
		i--;
	}
	//printf("i: %d\n", i);
	commands[i] = node; 

	int pipefd[count - 1][2];
	while ( j < count - 1)
	{
		if (pipe(pipefd[j]) == -1)
		{
			perror("pipe");
			return 1;
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
			return 1;
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
	return last_status;
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
		printf("Non-defined AST type %s \n", ast->cmd->args[0]);
		return 1;
	}

}
