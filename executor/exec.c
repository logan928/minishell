#include "../minishell.h"
#include <sys/wait.h>

int run_builtin(t_command *cmd)
{
	(void) cmd;
	printf("running builtins");
	return 0;
}


static void apply_redirs(t_redir *r)
{
	(void) r;
	printf("inside applying redirections");
}


static int exec_command(t_command *cmd)
{
	if(cmd->command_kind == BUILTIN)
		return(run_builtin(cmd));
	else
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			apply_redirs(cmd->redirs);
			//printf("..path...%s...arg %s...env %s \n", (cmd->args)[0], (cmd->args)[1], *(cmd->env));
			printf("..path...%s...arg %s...env \n", (cmd->args)[0], (cmd->args)[1]);
			execve(ft_get_cmd_path((cmd->args)[0]), cmd->args, cmd->env);
			perror("execve");
			exit(127);
		}
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return WEXITSTATUS(status);
		return 1;
	}
	
}

static int exec_pipeline(t_ast *ast)
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
		exit(exec_ast(ast->left));
	}


	pid_t right_pid = fork();
	if (right_pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		if (ast->right->type == AST_CMD && ast->right->cmd)
			apply_redirs(ast->right->cmd->redirs);
		exit(exec_ast(ast->right));
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

int exec_ast(t_ast *ast)
{
	if (!ast)
		return 0;

	if (ast -> type == AST_CMD)
		return (exec_command(ast->cmd));
	else if (ast->type == AST_PIPE)
		return (exec_pipeline(ast));
	else if (ast->type == AST_AND)
	{
		int left;

		left = exec_ast(ast->left);
		if(left == 0)
			return(exec_ast(ast->right));
		return left;
	}
	else if (ast->type == AST_OR)
	{
		int left;

		left = exec_ast(ast->left);
		if (left != 0)
			return(exec_ast(ast->right));
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