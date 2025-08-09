/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/09 14:48:33 by mkugan           ###   ########.fr       */
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
	char	*src;
	char	*word;
	int		pos;
	int		start;
	char	quote;
	t_token	*tokens;
}	t_lexer;

void	lex(t_lexer *lexer);
int		ft_check_syntax(t_lexer *lexer);
void	ft_free_lexer(t_lexer *lexer);

t_token	*ft_new_token(t_token_kind kind, char *data);
void	ft_add_token(t_token **head, t_token *token);

char	**ft_clone_env(char *envp[]);
void	ft_env(char *env[]);
void	ft_free_env(char *envp[]);

void	ft_sigint_handler(int sig);
void	ft_sigquit_trap(int sig);

#endif
