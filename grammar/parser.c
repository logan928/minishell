#include "../minishell.h"

// forward decls
t_ast *parse_pipeline(t_shell *shell, t_token **tokptr);
t_ast *parse_logical(t_shell *shell, t_token **tokptr);
t_ast *parse_command(t_shell *shell, t_token **tokptr);

t_ast *parse(t_shell *shell, t_token **tokptr_copy)
{
    return parse_logical(shell, tokptr_copy);
}

t_ast *parse_logical(t_shell *shell, t_token **tokptr)
{
    t_ast *left = parse_pipeline(shell, tokptr);
    while (*tokptr && 
          ((*tokptr)->token_kind == AND_IF || (*tokptr)->token_kind == OR_IF))
    {
        t_token_kind op = (*tokptr)->token_kind;
        *tokptr = (*tokptr)->next;
        t_ast *right = parse_pipeline(shell, tokptr);

        t_ast *node = calloc(1, sizeof(t_ast));//
        if(op == AND_IF)
            node->type = AST_AND;
        else
            node->type = AST_OR;
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

t_ast *parse_pipeline(t_shell *shell, t_token **tokptr)
{
    t_ast *left = parse_command(shell, tokptr);
    while (*tokptr && (*tokptr)->token_kind == PIPE)
    {
        *tokptr = (*tokptr)->next;
        t_ast *right = parse_command(shell, tokptr);

        t_ast *node = calloc(1, sizeof(t_ast));//
        node->type = AST_PIPE;
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
};

t_ast *parse_command(t_shell *shell, t_token **tokptr)
{
    if (*tokptr && (*tokptr)->token_kind == L_PAREN)
    {
        *tokptr = (*tokptr)->next; // skip '('
        t_ast *subtree = parse_logical(shell, tokptr);
        if (!*tokptr || (*tokptr)->token_kind != R_PAREN)
        {
            printf( "minishell: syntax error, missing ')'\n");//
			shell->parse_err = 2;
            return NULL;
        }
        *tokptr = (*tokptr)->next; // skip ')'

        t_ast *node = calloc(1, sizeof(t_ast));//
        node->type = AST_SUBSHELL;
        node->left = subtree;
        return node;
    }
    else
    {
        t_command *cmd = command_formatter(shell, tokptr);
        t_ast *node = calloc(1, sizeof(t_ast));//
        node->type = AST_CMD;
        node->cmd = cmd;
        return node;
    }
}


#include <stdio.h>

// helper to indent tree levels
//If you need to keep or modify the value safely, make a copy:
/*static void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}

static void print_command(t_shell *shell, t_command *cmd) // reuse your existing printer
{
    if (!cmd) return;
    	printf("CMD kind: ");

  	switch (cmd->command_kind)
    {
       case BUILTIN:     printf("BUILTIN"); break;
     	case EXTERNAL:    printf("EXTERNAL"); break;
     	default:          printf("OTHER"); break;
    } 
    if (cmd->args)
    {
		t_cmd_access cmd_access;
		if (cmd->command_kind == EXTERNAL)
		{
			cmd_access = ft_get_cmd_path(shell, cmd->args);
			printf("CMD_ACCESS: %d:%d:%d\n", cmd_access.exist, cmd_access.executable, cmd_access.is_dir);
		}
       printf(" | Args:");
        for (int i = 0; cmd->args[i]; i++)
			printf(" [%s]", cmd->args[i]);
   }
	printf("\n\t\t\tRedirs:");
	while (cmd->redirs)
	{
		printf(" [%d: %s]", cmd->redirs->kind, cmd->redirs->file);
		cmd->redirs = cmd->redirs->next;
	}
	printf("\n");
    //if (cmd->op && cmd->file)
      //  printf(" | Redir: %s %s", cmd->op, cmd->file);
    //printf("\n");
	
	
}

void print_ast(t_shell *shell, t_ast *node, int depth)
{
    if (!node) return;

    print_indent(depth);

    switch (node->type)
    {
        case AST_CMD:
            printf("AST_CMD:\n");
            print_indent(depth + 1);
            print_command(shell, node->cmd);
            break;
        case AST_PIPE:
            printf("AST_PIPE:\n");
            print_ast(shell, node->left, depth + 1);
            print_ast(shell, node->right, depth + 1);
            break;
        case AST_AND:
            printf("AST_AND:\n");
            print_ast(shell, node->left, depth + 1);
            print_ast(shell, node->right, depth + 1);
            break;
        case AST_OR:
            printf("AST_OR:\n");
            print_ast(shell, node->left, depth + 1);
            print_ast(shell, node->right, depth + 1);
            break;
        case AST_SUBSHELL:
            printf("AST_SUBSHELL:\n");
            print_ast(shell, node->left, depth + 1);
            break;
        default:
            printf("AST_UNKNOWN\n");
            break;
    }
}
*/
