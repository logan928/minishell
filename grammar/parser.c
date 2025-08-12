/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:48:42 by uwettasi          #+#    #+#             */
/*   Updated: 2025/08/07 15:48:55 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_ast	*parse_command(t_token **tokens);
static t_ast	*parse_pipeline(t_token **tokens);
static t_ast	*parse_logical(t_token **tokens);

static t_token	*eat_token(t_token **tokens) // Transfer: parser_Utils.c
{
	t_token	*cur = *tokens;
	if (cur)
		*tokens = cur->next;
	return (cur);
}

static t_ast	*new_ast_node(t_ast_type type)  // Transfer: parser_Utils.c
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
	{
		perror("Error building AST");
		exit(EXIT_FAILURE);
	}
	node->type = type;
	node->argv = NULL;
	node->file = NULL;
	node->redir_type = WORD;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;
	t_token	*tok;
	int		argc = 0;

	if (!*tokens || (*tokens)->token_kind != WORD)
		return (NULL);
	cmd = new_ast_node(AST_COMMAND);

	// Count args
	t_token	*tmp = *tokens;
	while (tmp && tmp->token_kind == WORD)
	{
		argc++;
		tmp = tmp->next;
	}

	cmd->argv = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->argv)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	argc = 0;
	while (*tokens && (*tokens)->token_kind == WORD)
	{
		tok = eat_token(tokens);
		cmd->argv[argc++] = tok->data;
		free(tok); // free token struct but not data
	}
	cmd->argv[argc] = NULL;
	return (cmd);
}


static t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left = parse_command(tokens);
	t_ast	*node;
	t_token	*tok;

	while (*tokens && (*tokens)->token_kind == PIPE)
	{
		tok = eat_token(tokens);
		free(tok->data);
		free(tok);
		node = new_ast_node(AST_PIPE);
		node->left = left;
		node->right = parse_command(tokens);
		left = node;
	}
	return (left);
}

static t_ast	*parse_logical(t_token **tokens)
{
	t_ast	*left = parse_pipeline(tokens);
	t_ast	*node;
	t_token	*tok;

	while (*tokens && ((*tokens)->token_kind == AND_IF
			|| (*tokens)->token_kind == OR_IF))
	{
		tok = eat_token(tokens);
		node = new_ast_node((*tok).token_kind == AND_IF ? AST_AND : AST_OR); // refactor
		free(tok->data);
		free(tok);
		node->left = left;
		node->right = parse_pipeline(tokens);
		left = node;
	}
	return (left);
}

/* Entry point */
t_ast	*parse_tokens(t_token **tokens)
{
	return (parse_logical(tokens));
}

void	free_ast(t_ast *node) // Transfer: parser_Utils.c
{
	int	i;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->argv)
	{
		for (i = 0; node->argv[i]; i++)
			free(node->argv[i]);
		free(node->argv);
	}
	if (node->file)
		free(node->file);
	free(node);
}

void	print_ast(t_ast *node, int depth)
{
	int	i;

	if (!node)
		return ;
	for (i = 0; i < depth; i++)
		printf("  ");
	printf("Node: ");
	if (node->type == AST_COMMAND)
	{
		printf("COMMAND");
		if (node->argv)
		{
			printf(" [");
			for (i = 0; node->argv[i]; i++)
			{
				printf("%s", node->argv[i]);
				if (node->argv[i + 1])
					printf(", ");
			}
			printf("]");
		}
		printf("\n");
	}
	else if (node->type == AST_PIPE)
		printf("PIPE\n");
	else if (node->type == AST_AND)
		printf("AND\n");
	else if (node->type == AST_OR)
		printf("OR\n");
	else
		printf("UNKNOWN\n");
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}
