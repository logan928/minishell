/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 14:45:06 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/06 14:45:10 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_expansion_cmd_child(t_shell *shell, t_command *cmd)
{
	ft_variable_expansion(shell, cmd->args, 0);
	ft_field_splitting(shell, &cmd->args, 0);
	ft_file_exp(shell, &cmd->args, 1, 1);
	ft_quote_removal(shell, cmd->args, 0);
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
	commands = malloc(sizeof(t_ast *) * count); // increase to count + 1
	if (!commands)
		return (NULL);
	i = count - 1;
	search_commands(&node, commands, &i);
	commands[i] = node; //check this gdb; commands count == NULL
	*pipe_count = count;//check
	return (commands);
}

int	get_fd_array(t_ast *ast, t_ast ***commands, int *count, int ***pipefd)
{
	int	j;
	int	pipe_count;

	j = 0;
	pipe_count = 0;
	*commands = get_leaf_commmands(ast, &pipe_count);
	if (!*commands)
		return (1);
	*count = pipe_count;//gdb
	*pipefd = malloc(sizeof(int *) * (pipe_count - 1));
	if (!*pipefd)
		return (1);
	while (j < pipe_count - 1)
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

//TODO: SHELL
pid_t	*get_pid_ts(t_pipe_parameters *tpp)
{
	pid_t	*pids;

	pids = malloc((tpp->count) * sizeof(pid_t));
	if (pids == NULL) 
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	return (pids);
}
