/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:11:05 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/06 16:11:18 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#include <stdlib.h>

static void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free_str_array(redir->file);
		free(redir->content);
		free(redir);
		redir = tmp;
	}
}

void	free_command(t_command *cmd)
{   
    //printf("inside_command\n");
	if (!cmd)
		return;
	if(cmd->path)
		free(cmd->path);
	free_str_array(cmd->args);
	free_str_array(cmd->env);
	free_redirs(cmd->redirs);
    //printf("outside_command\n");
	free(cmd);
}

void	free_ast(t_ast *node)
{
    //printf("inside\n");
	if (!node)
		return;
	free_ast(node->left);
	free_ast(node->right);
    if(node->cmd)
	    free_command(node->cmd);
	free(node);
}
