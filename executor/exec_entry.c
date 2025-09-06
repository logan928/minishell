/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_entry.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:48:18 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/06 15:48:22 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
