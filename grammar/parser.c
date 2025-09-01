/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:35:07 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/01 13:35:16 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*parse_pipeline(t_shell *shell, t_token **tokptr);
t_ast	*parse_logical(t_shell *shell, t_token **tokptr);
t_ast	*parse_command(t_shell *shell, t_token **tokptr);

t_ast	*parse(t_shell *shell, t_token **tokptr_copy)
{
	return (parse_logical(shell, tokptr_copy));
}

t_ast	*parse_logical(t_shell *shell, t_token **tokptr)
{
	t_ast			*left;
	t_token_kind	op;
	t_ast			*right;
	t_ast			*node;

	left = parse_pipeline(shell, tokptr);
	while (*tokptr && \
		((*tokptr)->token_kind == AND_IF || (*tokptr)->token_kind == OR_IF))
	{
		op = (*tokptr)->token_kind;
		*tokptr = (*tokptr)->next;
		right = parse_pipeline(shell, tokptr);
		node = ft_calloc(1, sizeof(t_ast));
		if (op == AND_IF)
			node->type = AST_AND;
		else
			node->type = AST_OR;
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

t_ast	*parse_pipeline(t_shell *shell, t_token **tokptr)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	left = parse_command(shell, tokptr);
	while (*tokptr && (*tokptr)->token_kind == PIPE)
	{
		*tokptr = (*tokptr)->next;
		right = parse_command(shell, tokptr);
		node = ft_calloc(1, sizeof(t_ast));
		node->type = AST_PIPE;
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

t_ast	*return_left_subtree(t_ast *node, t_ast_type type, t_ast *subtree)
{
	node->type = type;
	node->left = subtree;
	return (node);
}

t_ast	*parse_command(t_shell *shell, t_token **tokptr)
{
	t_ast		*subtree;
	t_command	*cmd;
	t_ast		*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (*tokptr && (*tokptr)->token_kind == L_PAREN)
	{
		*tokptr = (*tokptr)->next;
		subtree = parse_logical(shell, tokptr);
		if (!*tokptr || (*tokptr)->token_kind != R_PAREN)
		{
			shell->parse_err = 2;
			return (printf("minishell: syntax error, missing ')'\n"), NULL);
		}
		*tokptr = (*tokptr)->next;
		return (return_left_subtree(node, AST_SUBSHELL, subtree));
	}
	else
	{
		cmd = command_formatter(shell, tokptr);
		node->type = AST_CMD;
		node->cmd = cmd;
		return (node);
	}
}
