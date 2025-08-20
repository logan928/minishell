/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 17:42:04 by mkugan           ###   ########.fr       */
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
# include <sys/types.h>
# include <dirent.h>

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
	//BUILTIN, - //builtin/external to be identified at executor
	CMAND,
	LOGICAL,
	PIPELINE,
	PAREN,
	REDIRECTION,
}	t_lexem_kind;

/*typedef struct s_lexem 			//map with t_ast_type
{
	t_lexem_kind	lexem_kind;
	char			*path;
	char			**args;
	char			**env;
	char			*op;
	char			*file;
}	t_lexem; */

typedef struct s_lexer 			//map with t_ast
{
	char	*word;
	int		pos;
	int		start;
	char	quote;
	t_token	*tokens;
	t_token	*split_tmp;
}	t_lexer;

typedef struct s_shell
{
	char	*prompt;
	int		exit_status;
	t_lexer	*lexer;
	char	**env;
	char	*input;
}	t_shell;

char	*get_prompt(void);
void	ft_init_shell(t_shell *shell, char *envp[]);
void	ft_critical_error(t_shell *shell);

void	lex(t_shell *shell, char *input, t_lexer *lexer);
int		ft_check_syntax(t_lexer *lexer);
void	ft_variable_expansion(t_shell *shell);
int		ft_valid_env_char(int c);
char	*ft_get_env_var(t_shell *shell, char *s, size_t len, char **env);
void	ft_append_unquoted_quote(t_shell *s, t_token *t, char **res);
void	ft_append_quoted_quote(t_shell *s, t_token *t, char **res);
void	ft_append_variable(t_shell *s, t_token *t, char **res);
void	ft_append_exit_status(t_shell *s, char **res);
void	ft_append_normal_chars(t_shell *s, t_token *t, char **res);
void	ft_field_splitting(t_shell *shell);
void	ft_filename_expansion(t_shell *shell);
void	ft_quote_removal(t_shell *shell);
void	ft_free_lexer(t_lexer *lexer);
void	ft_reset_lexer(t_lexer *lexr);
void	ft_reset_lexer_cursor(t_lexer *lexer);

t_token	*ft_new_token(t_token_kind kind, char *data);
void	ft_add_token(t_token **head, t_token *token);
void	ft_insert_after(t_token *target, t_token *token);
void	ft_free_tokens(t_token *head);

char	**ft_clone_env(char *envp[]);
void	ft_env(char *env[]);
void	ft_free_env(char *envp[]);

void	ft_sigint_handler(int sig);
void	ft_sigquit_trap(int sig);

char	*ft_strdup_safe(t_shell *shell, const char *s);
char	*ft_strndup_safe(t_shell *shell, const char *s, size_t n);
t_token	*ft_new_token_safe(t_shell *shell, t_token_kind kind, char *data);
char	*ft_strjoin_free_safe(t_shell *shell, char *s1, char *s2);
char	*ft_itoa_safe(t_shell *shell, long n);

typedef enum e_ast_type
{
	AST_CMD, // seperate to built-in and external
	AST_PIPE,
	AST_AND,
	AST_OR,
	AST_SUBSHELL,
	AST_REDIR
}	t_ast_type;







typedef enum e_redir_type
{
	R_IN,     // <
	R_OUT,    // >
	R_APP,    // >>
	R_HDOC    // <<
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	kind;
	char			*file;     // filename or heredoc limiter
	char			*content; //What is usually under content??? heredocs?? TODO: handle content
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	t_lexem_kind	lexem_kind; //consider renaming as command_kind??
	char			*path;
	char			**args;   // argv for execve()
	char			**env;
	t_redir			*redirs;  // linked list of redirections
	//char			*op; //may not be needed if t_redir is used
	//char			*file; //may not be needed if t_redir is used
}	t_command;

typedef struct s_ast
{
	t_ast_type		type;
	//char			**argv;       // for commands : can include path
	//char			**env;		 // can also be inside argv
	//char			*file;        // for redirections
	t_command		*cmd;
	//t_token_kind	redir_type;   // <, >, <<, >>  :-- same as op in above
	struct s_ast	*left;        // for binary ops
	struct s_ast	*right;       // for binary ops
	int				ast_depth;	//tol: will this be useful for traversing?
}	t_ast;


t_command	*command_formatter(t_token **tokptr);
void	print_lexem(t_command *cmd);

int	ft_echo(char **args);

t_ast *parse(t_token **tokptr);
t_ast	*parse_tokens(t_token **tokens);
void	free_ast(t_ast *node);
void	print_ast(t_ast *node, int depth);

#endif
