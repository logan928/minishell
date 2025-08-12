/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/11 15:37:13 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include "libft/libft.h"

typedef struct s_token	t_token;
typedef struct s_lexem	t_lexem;

typedef enum e_token_kind
{
	WORD,
	L_PAREN,
	R_PAREN,
	PIPE,
	AND_IF,
	OR_IF,
	DLESS,
	LESS,
	GREAT,
	DGREAT,
	NL,
}	t_token_kind;

typedef struct s_token
{
	t_token_kind	token_kind;
	char			*data;
	t_token			*next;
}	t_token;

typedef enum e_lexem_kind
{
	BUILTIN,
	EXTERNAL,
	LOGICAL,
	PIPELINE,
	PAREN,
	REDIRECTION,
}	t_lexem_kind;

typedef struct s_lexem 			//map with t_ast_type
{
	t_lexem_kind	lexem_kind;
	char			*path;
	char			**args;
	char			**env;
	char			*op;
	char			*file;
}	t_lexem;

typedef struct s_lexer 			//map with t_ast
{
	char	*src;
	char	*word;
	int		pos;
	int		start;
	char	quote;
	t_token	*tokens;
	char	**env;
}	t_lexer;

void	lex(t_lexer *lexer);
int		ft_check_syntax(t_lexer *lexer);
void	ft_expand(t_lexer *l);
void	ft_free_lexer(t_lexer *lexer);
int		ft_valid_env_char(int c);
char	*ft_get_env_var(char *s, size_t len, char **env);

t_token	*ft_new_token(t_token_kind kind, char *data);
void	ft_add_token(t_token **head, t_token *token);

char	**ft_clone_env(char *envp[]);
void	ft_env(char *env[]);
void	ft_free_env(char *envp[]);

void	ft_sigint_handler(int sig);
void	ft_sigquit_trap(int sig);

typedef enum e_ast_type
{
	AST_COMMAND, // seperate to built-in and external
	AST_PIPE,
	AST_AND,
	AST_OR,
	AST_SUBSHELL,
	AST_REDIR
}	t_ast_type;

typedef struct s_ast
{
	t_ast_type		type;
	char			**argv;       // for commands : can include path
	char			**env;		 // can also be inside argv
	char			*file;        // for redirections
	t_token_kind	redir_type;   // <, >, <<, >>  :-- same as op in above
	struct s_ast	*left;        // for binary ops
	struct s_ast	*right;       // for binary ops
	int				ast_depth;
}	t_ast;

t_ast	*parse_tokens(t_token **tokens);
void	free_ast(t_ast *node);
void	print_ast(t_ast *node, int depth);



#endif
