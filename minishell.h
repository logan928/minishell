/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/10 17:05:54 by mkugan           ###   ########.fr       */
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
# include <sys/wait.h>
# include <fcntl.h>

# define EUNEXPTKN		"minishell: syntax error near unexpected token `"
# define ETMARGS		": too many arguments\n"
# define ENUMREQ		": numeric argument required\n"
# define UNEXPQOUT		"minishell: unexpected EOF while looking for matching`"

# define MAIN_SHELL		1
# define CHILD_SHELL	0

# define CMD_NOT_EXEC	126
# define CMD_NOT_FOUND	127

# define HEREDOC_EOF	1
# define HEREDOC_INT	130

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

typedef struct s_strvec
{
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
	bool	path;
}	t_cmd_access;

/*
	R_IN	<
	R_OUT	>
	R_APP	>>
	R_HDOC	<<
*/

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APP,
	R_HDOC
}	t_redir_type;

/*
 *	t_redir stores information about a rediridection.
 *	file has a value of the next WORD following a redirection operator.
 *	For heredoc it is user input. We transform a limiter into text
 *	before parsing.
 *	After expansion there could be arbitrary number of elements
 *	(except for heredoc). Only 1 is valid.
 *	TODO: content is not used and can be removed.
 */

typedef struct s_redir
{
	t_redir_type	kind;
	char			**file;
	char			*content;
	struct s_redir	*next;
}	t_redir;

typedef enum e_command_kind
{
	BUILTIN,
	EXTERNAL,
}	t_command_kind;

//TODO: We don't use env inside this struct
typedef struct s_command
{
	t_command_kind	command_kind;
	char			*path;
	char			**args;
	char			**env;
	t_redir			*redirs;
}	t_command;

//TODO: Do we need ast_depth?
typedef struct s_ast
{
	t_ast_type		type;
	t_command		*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
	int				ast_depth;
}	t_ast;

typedef struct s_glob_state
{
	int	last_star_index;
	int	star_match_index;
	int	in_quotes;
	int	pattern_index;
	int	filename_index;
	int	quoted_star;
}	t_glob_state;

typedef struct s_check_access_msgs
{
	char	*msg;
	bool	is_access_exists;
}	t_cehck_access_msgs;

typedef struct s_pipe_parameters
{
	int		count;
	int		status;
	int		last_status;
	bool	new_line;
	bool	core_dump;
	int		sig;
	int		**pipefd;
}	t_pipe_parameters;

char			*ft_set_prompt(t_shell *shell);
void			ft_init_shell(t_shell *shell, char *envp[]);
void			ft_critical_error(t_shell *shell);
void			lex(t_shell *shell, char *input);
int				ft_check_syntax(t_shell *shell);
int				ft_valid_env_char(int c);
char			*ft_get_env_var(t_shell *shell, char *s);
void			ft_app_uquote(t_shell *s, t_cursor *c, char *t, char **res);
void			ft_app_qquote(t_shell *s, t_cursor *c, char *t, char **res);
void			ft_app_var(t_shell *s, t_cursor *c, char *t, char **res);
void			ft_app_exit(t_shell *s, t_cursor *c, char **res);
void			ft_app_char(t_shell *s, t_cursor *c, char *t, char **res);
void			ft_free_lexer(t_lexer *lexer);
int				ft_pattern_match(const char *pattern, const char *filename);
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
char			*fts_strdup(t_shell *shell, const char *s);
char			*fts_strndup(t_shell *shell, const char *s, size_t n);
t_token			*fts_new_token(t_shell *shell, t_token_kind kind, char *data);
char			*fts_strjoin_free(t_shell *shell, char *s1, char *s2);
char			*fts_itoa(t_shell *shell, long n);
void			*fts_malloc(t_shell *shell, size_t size);
bool			ft_is_valid_number(char *s);
void			ft_num_arg_req(t_shell *shell, char *cmd, char *arg);
void			ft_not_set(t_shell *shell, char *var);
char			*ft_get_pwd(t_shell *shell);
void			ft_variable_expansion(t_shell *shell, char **args, size_t idx);
void			ft_field_splitting(t_shell *shell, char ***arr, size_t idx);
size_t			ft_arr_size(char **arr);
size_t			ft_lst_size(t_token *tokens);
void			ft_free_arr(char **arr);
void			ft_merge(t_shell *sh, char ***arr, size_t lst_size, int is_cmd);
void			ft_file_exp(t_shell *sh, char ***arr, size_t idx, int is_cmd);
void			ft_quote_removal(t_shell *shell, char **args, size_t idx);
t_cmd_access	ft_get_cmd_path(t_shell *shell, t_command *cmd);
void			ft_here_doc(t_shell *shell, t_token *t);
void			ft_quote_removal_str(t_shell *shell, t_token *t);
void			ft_here(t_shell *shell);
t_command		*command_formatter(t_shell *shell, t_token **tokptr);
void			print_lexem(t_command *cmd);
t_ast			*parse(t_shell *shell, t_token **tokptr_copy);
t_ast			*parse_tokens(t_token **tokens);
void			free_ast(t_ast *node);
void			print_ast(t_shell *shell, t_ast *node, int depth);
char			*fts_strjoin3cpy(t_shell *shell, char *s1, char *s2, char *s3);
void			fts_write(t_shell *shell, char *s, int fd);
int				exec_ast(t_shell *shell, t_ast *ast);
void			ft_cd_too_many_args(t_shell *shell);
char			*ft_canonicalize(t_shell *shell, char *curpath);
char			*ft_reconstruct_path(t_stack *st);
void			ft_unset(t_shell *shell, char **args);
char			**ft_clone_env_and_return(t_shell *shell, char *envp[]);
int				ft_valid_env_first_char(int c);
void			ft_export(t_shell *shell, char **args);
bool			ft_is_valid_var_name(char *s);
void			ft_skip_empty_vars(t_shell *shell, char **args);
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
void			ft_sigint_handler_here(int sig);
void			ft_set_here_sigint(void);
int				open_file(t_redir *redir, int shell_type, int flags);
int				handle_redir(t_shell *shell, t_redir *redir, int shell_type, \
				t_command_kind kind);
void			ft_init_access(t_shell *shell, t_cmd_access *access);
int				ft_check_access(t_shell *shell, t_command *cmd);
void			ft_chdir_err(t_shell *shell, char *dir);
int				ft_first_unquoted_char(const char *pattern);
int				ft_isoperator(t_token_kind kind);
int				ft_isredirection(t_token_kind kind);
int				ft_syntax_error(t_shell *shell, char *token);
int				apply_redirs(t_shell *shell, t_redir *redir, \
				t_command_kind kind, int shell_type);
// int				exec_command_builtins(t_shell *shell, t_command *cmd);
int				exec_command_externals(t_shell *shell, t_command *cmd);
void			exec_command_expansions(t_shell *shell, t_command *cmd);
void			handle_expansion_cmd_child(t_shell *shell, t_command *cmd);
int				get_fd_array(t_ast *ast, t_ast ***commands, int *count, \
				int ***pipefd);
int				exec_pipeline_core (t_shell *shell, int ***pipefd, \
				t_ast ***commands, t_pipe_parameters *tpp);
int				run_builtin(t_shell *shell, t_command *cmd, int shell_type);
pid_t			*get_pid_ts(t_shell *shell, t_pipe_parameters *tpp);
int 			exec_command_builtins(t_shell *shell, t_command *cmd);
int				exec_command(t_shell *shell, t_command *cmd);
int				exec_pipeline(t_shell *shell, t_ast *ast);
int				exec_subshell(t_shell *shell, t_ast *ast);
int				handle_and(t_shell *shell, t_ast *ast);
int				handle_or(t_shell *shell, t_ast *ast);
void			ft_set_signals_main_pre(void);
void			ft_set_signals_hd_pre(void);
void			ft_set_signals_post(void);
void			ft_free_exit(t_shell *shell);
void			free_command(t_command *cmd);
void			ft_critical_with_code(t_shell *shell, int code, t_ast **commands);
int				ft_heredoc_file(t_shell *shell, int w, int r, char *input);
void 			free_tpp(t_pipe_parameters *tpp, int fd_count);
void			free_commands(t_ast **commands);

#endif
