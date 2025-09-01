/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 18:22:06 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
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
# include <stdbool.h>
# include <sys/stat.h>
# include <errno.h>

# define EUNEXPTKN "minishell: syntax error near unexpected token `"
# define ETMARGS ": too many arguments\n"
# define ENUMREQ ": numeric argument required\n"
# define ENOHOME ": HOME not set\n"

# define MAIN_SHELL 1
# define CHILD_SHELL 0

# define CMD_NOT_EXEC	126
# define CMD_NOT_FOUND	127

typedef struct s_token	t_token;
typedef struct s_lexem	t_lexem;
typedef struct s_ast	t_ast;

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

typedef struct s_strvec {
	char	**data;
	size_t	len;
	size_t	cap;
}	t_strvec;

typedef struct s_cursor
{
	size_t	cur;
	size_t	start;
	char	quote;
}	t_cursor;

typedef struct s_token
{
	t_token_kind	token_kind;
	char			*data;
	t_token			*next;
}	t_token;

typedef struct s_lexer
{
	char	*word;
	bool	io_here;
	t_token	*tokens;
	t_token	*tmp;
	t_token	*tmp2;
}	t_lexer;

typedef struct s_shell
{
	char			*prompt;
	unsigned char	exit_status;
	t_lexer			*lexer;
	t_strvec		*env;
	t_strvec		*exp;
	char			*input;
	char			*pwd;
	t_ast			*ast;
	int				parse_err;
}	t_shell;

typedef enum e_ast_type
{
	AST_CMD,
	AST_PIPE,
	AST_AND,
	AST_OR,
	AST_SUBSHELL,
	AST_REDIR
}	t_ast_type;

typedef struct s_cmd_access
{
	bool	exist;
	bool	executable;
	bool	is_dir;
}	t_cmd_access;

typedef enum e_redir_type
{
	R_IN,     // <
	R_OUT,    // >
	R_APP,    // >>
	R_HDOC   // <<
}	t_redir_type;


// TODO: the idea is to store filename/heredoc content as a string array, so we can pass it to expansion functions
typedef struct s_redir
{
	t_redir_type	kind;
	char			**file;     // filename or heredoc limiter
	char			*content; //What is usually under content??? heredocs?? TODO: handle content
	struct s_redir	*next;
}	t_redir;

typedef enum e_command_kind
{
	BUILTIN, 
	EXTERNAL,
}	t_command_kind;

typedef struct s_command
{
	t_command_kind	command_kind; //consider renaming as command_kind??
	char			*path;
	char			**args;   // argv for execve()
	char			**env;
	t_redir			*redirs;  // linked list of redirections
}	t_command;

typedef struct s_ast
{
	t_ast_type		type;
	t_command		*cmd;
	struct s_ast	*left;        // for binary ops
	struct s_ast	*right;       // for binary ops
	int				ast_depth;	//tol: will this be useful for traversing?
}	t_ast;

typedef struct s_glob_state
{
	int	last_star_index;
	int	star_match_index;
	int	in_quotes;
	int	pattern_index;
	int	filename_index;
}	t_glob_state;

char			*ft_set_prompt(t_shell *shell);
void			ft_init_shell(t_shell *shell, char *envp[]);
void			ft_critical_error(t_shell *shell);
void			lex(t_shell *shell, char *input);
int				ft_check_syntax(t_shell *shell);
int				ft_valid_env_char(int c);
char			*ft_get_env_var(t_shell *shell, char *s);
void			ft_append_unquoted_quote(t_shell *s, t_cursor *c, char *t, char **res);
void			ft_append_quoted_quote(t_shell *s, t_cursor *c, char *t, char **res);
void			ft_append_variable(t_shell *s, t_cursor *c, char *t, char **res);
void			ft_append_exit_status(t_shell *s, t_cursor *c, char **res);
void			ft_append_normal_chars(t_shell *s, t_cursor *c, char *t, char **res);
void			ft_free_lexer(t_lexer *lexer);
int				ft_pattern_match(const char *pattern, const char *filename);
int				ft_is_pattern(char *word);
t_token			*ft_new_token(t_token_kind kind, char *data);
void			ft_add_token(t_token **head, t_token *token);
void			ft_insert_after(t_token *target, t_token *token);
void			ft_free_tokens(t_token *head);
void			ft_add_token_sorted(t_token **head, t_token *token);
void			ft_clone_env(t_shell *shell, char *envp[]);
void			ft_env(t_shell *shell, char *env[]);
void			ft_free_env(char *envp[]);
void			ft_exit(t_shell *shell, char **args, int shell_type);
void			ft_echo(t_shell *shell, char **args);
char			*ft_get_cwd(t_shell *shell);
void			ft_pwd(t_shell *shell, char **args);
void			ft_cd(t_shell *shell, char **args);
void			ft_set_pwd(t_shell *shell);
void			ft_sigint_handler(int sig);
void			ft_sigquit_trap(int sig);
void			ft_too_many_args(t_shell *shell, char *cmd, unsigned char exit);
char			*ft_strdup_safe(t_shell *shell, const char *s);
char			*ft_strndup_safe(t_shell *shell, const char *s, size_t n);
t_token			*ft_new_token_safe(t_shell *shell, t_token_kind kind, char *data);
char			*ft_strjoin_free_safe(t_shell *shell, char *s1, char *s2);
char			*ft_itoa_safe(t_shell *shell, long n);
void			*ft_malloc_safe(t_shell *shell, size_t size);
bool			ft_is_valid_number(char *s);
void			ft_num_arg_req(t_shell *shell, char *cmd, char *arg);
void			ft_home_not_set(t_shell *shell, char *cmd, char *tmp);
char			*ft_get_pwd(t_shell *shell);
void			ft_variable_expansion(t_shell *shell, char **args, size_t idx);
void			ft_field_splitting(t_shell *shell, char ***arr, size_t idx);
size_t			ft_arr_size(char **arr);
size_t			ft_lst_size(t_token *tokens);
void			ft_free_arr(char **arr);
void			ft_merge(t_shell *shell, char ***arr, size_t lst_size, int is_cmd);
void			ft_filename_expansion(t_shell *shell, char ***arr, size_t idx, int is_cmd);
void			ft_quote_removal(t_shell *shell, char **args, size_t idx);
t_cmd_access	ft_get_cmd_path(t_shell *shell, char **args);
void			ft_here_doc(t_shell *shell, t_token *t);
void			ft_quote_removal_str(t_shell *shell, t_token *t);
void			ft_here(t_shell *shell);
t_command		*command_formatter(t_shell *shell, t_token **tokptr);
void			print_lexem(t_command *cmd);
t_ast			*parse(t_shell *shell, t_token **tokptr_copy);
t_ast			*parse_tokens(t_token **tokens);
void			free_ast(t_ast *node);
void			print_ast(t_shell *shell, t_ast *node, int depth);
char			*ft_str_join3_cpy_safe(t_shell *shell, char *s1, char *s2, char *s3);
void			ft_write_safe(t_shell *shell, char *s, int fd);
int				exec_ast(t_shell *shell, t_ast *ast);
void			ft_cd_too_many_args(t_shell *shell);
char			*ft_canonicalize(t_shell *shell, char *curpath);
char			*ft_reconstruct_path(t_stack *st);
void			ft_unset(t_shell *shell, char **args);
char			**ft_clone_env_and_return(t_shell *shell, char *envp[]);
int				ft_valid_env_first_char(int c);
void			ft_export(t_shell *shell, char **args);
bool 			ft_is_valid_var_name(char *s);
void			ft_skip_empty_vars(t_shell *shell);
int				is_builtin(const char *cmd);
char			**argv_add(char **argv, int *argc, const char *word);
t_command		*command_new(void);
t_strvec		*ft_strvec_init(size_t cap);
void			ft_strvec_free(t_strvec *sv);
t_strvec		*ft_strvec_push(t_strvec **sv, char *s);
t_strvec		*ft_strvec_update(t_strvec *sv, char *s, char *val);
size_t			ft_strvec_remove(t_strvec *sv, const char *s);
char			*ft_strvec_getval(const t_strvec *sv, const char *s);
ssize_t			ft_strvec_find(const t_strvec *sv, const char *s);
void			ft_not_valid_identifier(t_shell *shell, char *arg);
t_token_kind	ft_get_token_kind(const char *s);
int				ft_get_operator_length(t_token_kind kind);
int				ft_is_normal_char(char c);
int				ft_is_operator_char(char c);
t_strvec		*ft_strvec_realloc(t_strvec *sv);
void			ft_shlvl(t_shell *shell);

typedef enum e_lexem_kind
{
	//BUILTIN, - //builtin/external to be identified at executor
	CMAND,
	LOGICAL,
	PIPELINE,
	PAREN,
	REDIRECTION,
}	t_lexem_kind;
#endif
