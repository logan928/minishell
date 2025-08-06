/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/06 14:19:48 by mkugan           ###   ########.fr       */
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

typedef struct s_token	t_token;

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

t_token	*ft_new_token(t_token_kind kind, char *data);
void	ft_add_token(t_token **head, t_token *token);

char	**ft_clone_env(char *envp[]);
void	ft_env(char *env[]);

void	ft_sigint_handler(int sig);
void	ft_sigquit_trap(int sig);

#endif
