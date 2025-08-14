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
	t_token	*cur;

	if (!tokens || !*tokens)
		return (NULL);
	cur = *tokens;
	*tokens = cur->next;
	cur -> next = NULL;
	return (cur);
}

static  int is_redir_kind(t_token_kind k){
	return(k == LESS || k == GREAT || k == DLESS || k == DGREAT);
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

typedef struct  s_arg_node //transfer: .h 
{
	char    *arg;
	struct s_arg_node *next;
} t_arg_node;

static void	arg_push(t_arg_node **head, char *s) // Transfer: parse_utilities.c
{
	t_arg_node *node;

	node = malloc(sizeof(t_arg_node));
	if (!node)
	{
		perror("Error building AST");
		exit(EXIT_FAILURE);
	}
	node->arg = s;
	node->next = NULL;
	if (!*head)
		*head = node;
	else
	{
		t_arg_node *it = *head;
		while (it->next)
			it = it->next;
		it->next = node;
	}
}

typedef struct s_redir_node // Transfer : .h
{
	t_token_kind			kind;
	char					*file;
	struct s_redir_node		*next;
}	t_redir_node;

static void	redir_push(t_redir_node **head, t_token_kind kind, char *file)
{
	t_redir_node *n;

	n = malloc(sizeof(t_redir_node));
	if (!n)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	n->kind = kind;
	n->file = file;
	n->next = NULL;
	if (!*head)
		*head = n;
	else
	{
		t_redir_node *it = *head;
		while (it->next)
			it = it->next;
		it->next = n;
	}
}

/* free temporary lists but NOT the strings they hold (strings go into AST)???? check how the expander works */
static void	free_arg_list(t_arg_node *head)
{
	t_arg_node *tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

static void	free_redir_list(t_redir_node *head)
{
	t_redir_node *tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}


static char	**arglist_to_argv(t_arg_node *head)
{
	int count = 0;
	t_arg_node *it = head;
	char **argv;
	int i = 0;

	while (it)
	{
		count++;
		it = it->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	it = head;
	while (it)
	{
		argv[i++] = it->arg;
		it = it->next;
	}
	argv[i] = NULL;
	free_arg_list(head);
	return (argv);
}

/* Wrap base (i.e. the root node of sub-tree) with redirections in order (first encountered becomes outermost wrapper)  
- what to do with the file? is it a branch or just an attribute of the node?*/
static t_ast	*apply_redirs(t_redir_node *redirs, t_ast *base)
{
	t_redir_node *it = redirs;
	t_ast *rnode;

	while (it)
	{
		rnode = new_ast_node(AST_REDIR);
		rnode->redir_type = it->kind;
		rnode->file = it->file; /* take ownership of the file string */
		rnode->left = base;
		base = rnode;
		it = it->next;
	}
	free_redir_list(redirs);
	return (base);
}

/*the following commented function does not handle redirections. For quick thests we can use this one. */
/*
static t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;
	t_token	*tok;
	int		argc = 0;

	if (!*tokens || (*tokens)->token_kind != WORD)
		return (NULL);
	cmd = new_ast_node(AST_COMMAND);

	t_token	*tmp = *tokens;
	while (tmp && tmp->token_kind == WORD)
	{
		argc++;
		tmp = tmp->next;
	}

	cmd->argv = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->argv)
	{
		perror("Error building AST");
		exit(EXIT_FAILURE);
	}
	argc = 0;
	while (*tokens && (*tokens)->token_kind == WORD)
	{
		tok = eat_token(tokens);
		cmd->argv[argc++] = tok->data;
		free(tok); // free the token struct but not data
	}
	cmd->argv[argc] = NULL;
	return (cmd);
} */

/*
 * parse_command:
 * Handles:
 *   - words (command + args)
 *   - subshells: '(' logical ')'
 *   - redirections placed before/after words/subshell

 */
static t_ast	*parse_command(t_token **tokens)
{
	t_arg_node		*args = NULL;
	t_redir_node	*redirs = NULL;
	t_ast			*base = NULL;
	t_token			*tok;

	/* keep eating tokens until we hit a pipeline/operator/paren close or end */
	while (*tokens)
	{
		t_token_kind k = (*tokens)->token_kind;
		if (k == WORD)
		{
			tok = eat_token(tokens); 
			arg_push(&args, tok->data);
			free(tok); /* free token struct, keep tok->data for AST : Not sure this is  a good practice*/
		}
		else if (is_redir_kind(k))
		{
			/* redirection operator followed by a WORD (filename/delimiter) */
			tok = eat_token(tokens); 
			/* after operator we must have a WORD (filename/delimiter) ???? check this logic with Mikhail*/
			if (!*tokens || (*tokens)->token_kind != WORD)
			{
				printf(stderr, "syntax error near unexpected token `%s'\n", tok->data); // use ft_printf
				free(tok->data);
				free(tok);
				free_arg_list(args);
				free_redir_list(redirs);
				return (NULL);
			}
			t_token *file_tok = eat_token(tokens);
			redir_push(&redirs, k, file_tok->data);
			free(file_tok);      /* keep data pointer */
			free(tok->data);     /* we don't need operator text ??*/
			free(tok);
		}
		else if (k == L_PAREN)
		{
			/* subshell: consume '(' then parse logical then expect ')' */
			tok = eat_token(tokens); /* this will get the left '(' token */
			free(tok->data);
			free(tok);
			t_ast *inner = parse_logical(tokens); //recursion
			if (!inner)
			{
				free_arg_list(args);
				free_redir_list(redirs);
				return (NULL);
			}
			/* looking for ) */
			if (!*tokens || (*tokens)->token_kind != R_PAREN)
			{
				printf(stderr, "syntax error: missing ')'\n");// use ft_printf
				free_ast(inner);
				free_arg_list(args);
				free_redir_list(redirs);
				return (NULL);
			}
			t_token *rpar = eat_token(tokens);
			free(rpar->data);
			free(rpar);

			base = new_ast_node(AST_SUBSHELL);
			base->left = inner;
			/* after subshell we continue to gather redirections or end */
		}
		else
		{
			/* stop parsing command on PIPE/AND/OR/R_PAREN/etc. */
			break;
		}
	}

	/* if we collected words -> create command node (or add words to existing base if subshell? we choose to treat them as command only) */
	if (args)
	{
		/* If base is subshell but also words were found (invalid), we'll create command instead.
		   NOTE: (Check this??) shell grammar doesn't allow a word immediately after a subshell without operator, so
		   we treat words as a command (i.e., '( ... ) ls' is separate tokens usually). */
		t_ast *cmd = new_ast_node(AST_COMMAND);
		cmd->argv = arglist_to_argv(args);
		base = (base ? base : cmd);// refactor
		/* If base existed (subshell) and we also parsed args, we should treat that as a separate command.
		    the last created node taken as as base (cmd). If need stricter validation,
		   detect and report syntax error here.???? */
		if (base != cmd && cmd->argv)
		{
			/* attach cmd as right child of a PIPE? This is context-specific; we keep base==cmd */
			/* free previous subshell -- choose simpler behavior: free subshell and return command */
			/* But to avoid surprise, let's keep command as base and free the previous subshell */
			free_ast(base);
			base = cmd;
		}
	}



	/* if we never created a base but have redirections, that's a syntax error (like: '< file | ...') */
	if (!base && redirs)
	{
		printf(stderr, "syntax error: redirection without command\n"); // use ft_printf
		free_redir_list(redirs);
		return (NULL);
	}

	/* apply collected redirections (wrap base in AST_REDIR nodes) */
	if (redirs)
		base = apply_redirs(redirs, base);

	return (base);
}


/* parse_pipeline: command ('|' command)* */
static t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;
	t_token	*tok;

	left = parse_command(tokens);
	if (!left)
		return (NULL);

	while (*tokens && (*tokens)->token_kind == PIPE)
	{
		tok = eat_token(tokens);
		free(tok->data);
		free(tok);
		right = parse_command(tokens);
		if (!right)
		{
			printf(stderr, "syntax error: expected command after '|'\n");//use ft_printf
			free_ast(left);
			return (NULL);
		}
		node = new_ast_node(AST_PIPE);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

/* parse_logical: pipeline ( (AND_IF|OR_IF) pipeline )* start of recutsion */
static t_ast	*parse_logical(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;
	t_token	*tok;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);

	while (*tokens && ((*tokens)->token_kind == AND_IF || (*tokens)->token_kind == OR_IF))
	{
		tok = eat_token(tokens);
		t_ast_type type = (tok->token_kind == AND_IF) ? AST_AND : AST_OR; // refactor
		free(tok->data);
		free(tok);
		right = parse_pipeline(tokens);
		if (!right)
		{
			printf(stderr, "syntax error: expected pipeline after logical operator\n");
			free_ast(left);
			return (NULL);
		}
		node = new_ast_node(type);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}



/* Entry point */
t_ast	*parse_tokens(t_token **tokens)
{
	t_ast *root = parse_logical(tokens);

	/* This function intends to ....If there are leftover tokens (unexpected), treat as syntax error */
	/* This does not work. Check 
	if (root && *tokens)
	{
		// if leftover are just R_PAREN this is likely handled elsewhere, but we conservatively report 
		fprintf(stderr, "syntax error: unexpected token after parsing\n");
		// don't free root here: caller may want to inspect; but to be safe we free and return NULL 
		free_ast(root);
		return (NULL);
	}
	*/
	return (root);
}




void	free_ast(t_ast *node) 
{
	int i;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->argv)
	{
		for (i = 0; node->argv[i]; i++) // refactor to while
			free(node->argv[i]);
		free(node->argv);
	}
	if (node->file)
		free(node->file);
	free(node);
}

static const char	*redir_name(t_token_kind k)
{
	if (k == LESS) return ("<");
	if (k == GREAT) return (">");
	if (k == DLESS) return ("<<");
	if (k == DGREAT) return (">>");
	return ("?");
}

void	print_ast(t_ast *node, int depth) // The same logic can be used to traverse the tree in Executor
{
	int i;

	if (!node)
		return;
	for (i = 0; i < depth; i++) // Replace with while
		printf("  ");//ft_printf
	switch (node->type) // replace with if...else if....
	{
		case AST_COMMAND:
			printf("COMMAND");
			if (node->argv)
			{
				printf(" [");
				for (i = 0; node->argv[i]; i++)
				{
					if (i) printf(", ");
					printf("%s", node->argv[i]);
				}
				printf("]");
			}
			printf("\n");
			break;
		case AST_PIPE:
			printf("PIPE\n");
			break;
		case AST_AND:
			printf("AND\n");
			break;
		case AST_OR:
			printf("OR\n");
			break;
		case AST_SUBSHELL:
			printf("SUBSHELL\n");
			break;
		case AST_REDIR:
			printf("REDIR %s -> %s\n", redir_name(node->redir_type),
				   node->file ? node->file : "(null)");
			break;
		default:
			printf("UNKNOWN\n");
			break;
	}
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}
