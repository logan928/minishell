/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/19 14:13:43 by mkugan           ###   ########.fr       */
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
	BUILTIN,
	EXTERNAL,
	LOGICAL,
	PIPELINE,
	PAREN,
	REDIRECTION,
}	t_lexem_kind;


typedef struct s_lexem
{
	t_lexem_kind	lexem_kind;
	char			*path;
	char			**args;
	char			**env;
	char			*op;
	char			*file;
}	t_lexem;

typedef struct s_lexer
{
	char	*word;
	int		pos;
	int		start;
	char	quote;
	t_token	*tokens;
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

void	lex(t_shell *shell);
int		ft_check_syntax(t_lexer *lexer);
void	ft_vars_expansion(t_shell *shell);
void	ft_field_splitting(t_shell *shell);
void	ft_filename_expansion(t_shell *shell);
void	ft_quote_removal(t_shell *shell);
void	ft_free_lexer(t_lexer *lexer);
int		ft_valid_env_char(int c);
char	*ft_get_env_var(char *s, size_t len, char **env);
void	ft_reset_lexer(t_lexer *lexr);

t_token	*ft_new_token(t_token_kind kind, char *data);
void	ft_add_token(t_token **head, t_token *token);
void	ft_insert_after(t_token *target, t_token *token);
void	ft_free_tokens(t_token *head);

char	**ft_clone_env(char *envp[]);
void	ft_env(char *env[]);
void	ft_free_env(char *envp[]);

void	ft_sigint_handler(int sig);
void	ft_sigquit_trap(int sig);

int	ft_echo(char **args);

#endif
