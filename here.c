/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:15:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 17:42:34 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_abort = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_abort = HEREDOC_INT;
	write(STDOUT_FILENO, "\n", 1);
}

static bool	ft_is_quoted(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (ft_isquote(s[i]))
			return (true);
		i++;
	}
	return (false);
}

void	ft_set_here_sigint(void)
{
	struct sigaction	sa1;

	sa1.sa_handler = sigint_handler;
	sigemptyset(&sa1.sa_mask);
	sa1.sa_flags = 0;
	sigaction(SIGINT, &sa1, NULL);
}

void	ft_here_eof_warning(t_shell *shell, char *limiter)
{
	char	*err;

	if (!g_abort)
	{
		g_abort = HEREDOC_EOF;
		err = ft_str_join3_cpy_safe(shell,
		"\nminishell: warning: here-document delimited by end-of-file (wanted `",
		limiter,
		"')\n");
		ft_write_safe(shell, err, STDERR_FILENO);
	}
}

void	ft_read_line(t_shell *shell, char **res, char *limiter)
{
	char	*line;
	size_t	limiter_len;

	ft_set_here_sigint();
	limiter_len = ft_strlen(limiter);
	while (1)
	{
		if (write(1, "> ", 2) != 2)
			ft_critical_error(shell);
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_here_eof_warning(shell, limiter);
			break ;
		}
		if (ft_strncmp(line, limiter, limiter_len) == 0
			&& line[limiter_len] == '\n')
		{
			free(line);
			break ;
		}
		*res = ft_strjoin_free_safe(shell, *res, line);
	}
}

void	ft_here_doc(t_shell *shell, t_token *t)
{
	bool	is_quoted;
	char	*res;

	is_quoted = ft_is_quoted(t->data);
	if (is_quoted)
		ft_quote_removal_str(shell, t);
	res = NULL;
	ft_read_line(shell, &res, t->data);
	if (g_abort == HEREDOC_INT)
	{
		free(res);
		return ;
	}
	if (!res)
		res = ft_strdup_safe(shell, "");
	if (is_quoted)
	{
		res = ft_strjoin_free_safe(shell, ft_strdup_safe(shell, "'"), res);
		res = ft_strjoin_free_safe(shell, res, ft_strdup_safe(shell, "'"));
	}
	free(t->data);
	t->data = res;
}

void	ft_here(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->lexer->tokens;
	while (tmp)
	{
		if (g_abort == HEREDOC_INT)
			break ;
		if (tmp->token_kind == DLESS)
			shell->lexer->io_here = true;
		else if (shell->lexer->io_here)
		{
			ft_here_doc(shell, tmp);
			shell->lexer->io_here = false;
		}
		tmp = tmp->next;
	}
}
