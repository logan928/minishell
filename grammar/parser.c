#include "../minishell.h"

// forward decls
t_ast *parse_pipeline(t_token **tokptr);
t_ast *parse_logical(t_token **tokptr);
t_ast *parse_command(t_token **tokptr);

t_ast *parse(t_token **tokptr)
{
    return parse_logical(tokptr);
}

t_ast *parse_logical(t_token **tokptr)
{
    t_ast *left = parse_pipeline(tokptr);
    while (*tokptr && 
          ((*tokptr)->token_kind == AND_IF || (*tokptr)->token_kind == OR_IF))
    {
        t_token_kind op = (*tokptr)->token_kind;
        *tokptr = (*tokptr)->next;
        t_ast *right = parse_pipeline(tokptr);

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

t_ast *parse_pipeline(t_token **tokptr)
{
    t_ast *left = parse_command(tokptr);
    while (*tokptr && (*tokptr)->token_kind == PIPE)
    {
        *tokptr = (*tokptr)->next;
        t_ast *right = parse_command(tokptr);

        t_ast *node = calloc(1, sizeof(t_ast));//
        node->type = AST_PIPE;
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
};

t_ast *parse_command(t_token **tokptr)
{
    if (*tokptr && (*tokptr)->token_kind == L_PAREN)
    {
        *tokptr = (*tokptr)->next; // skip '('
        t_ast *subtree = parse_logical(tokptr);
        if (!*tokptr || (*tokptr)->token_kind != R_PAREN)
        {
            printf( "minishell: syntax error, missing ')'\n");//
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
        t_command *cmd = command_formatter(tokptr);
        t_ast *node = calloc(1, sizeof(t_ast));//
        node->type = AST_CMD;
        node->cmd = cmd;
        return node;
    }
}


#include <stdio.h>

// helper to indent tree levels
static void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}
static void print_command(t_command *lex) // reuse your existing printer
{
    if (!lex) return;
    printf("Lexem kind: ");
    
    switch (lex->command_kind)
    {
        case BUILTIN:     printf("BUILTIN"); break;
        case EXTERNAL:    printf("EXTERNAL"); break;
        default:          printf("OTHER"); break;
    } 
    if (lex->args)
    {
        printf(" | Args:");
        for (int i = 0; lex->args[i]; i++)
            printf(" [%s]", lex->args[i]);
    }
    //if (lex->op && lex->file)
        //printf(" | Redir: %s %s", lex->op, lex->file);
    printf("\n");
}

void print_ast(t_ast *node, int depth)
{
    if (!node) return;

    print_indent(depth);

    switch (node->type)
    {
        case AST_CMD:
            printf("AST_CMD:\n");
            print_indent(depth + 1);
            print_command(node->cmd);
            break;
        case AST_PIPE:
            printf("AST_PIPE:\n");
            print_ast(node->left, depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case AST_AND:
            printf("AST_AND:\n");
            print_ast(node->left, depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case AST_OR:
            printf("AST_OR:\n");
            print_ast(node->left, depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case AST_SUBSHELL:
            printf("AST_SUBSHELL:\n");
            print_ast(node->left, depth + 1);
            break;
        default:
            printf("AST_UNKNOWN\n");
            break;
    }
}
