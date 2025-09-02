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

volatile sig_atomic_t g_abort = 0;

/* Signal handler: only sets a flag and writes a newline */
void sigint_handler(int sig)
{
    (void)sig;
    g_abort = 1;
    write(STDOUT_FILENO, "\n", 1); // async-signal-safe
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

void	ft_read_line(t_shell *shell, char **res, char *limiter)
{
	char	*line;
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; /* important: disables SA_RESTART */
    sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		if (write(1, "> ", 2) != 2)
			ft_critical_error(shell);
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
		{
			dprintf(STDERR_FILENO,
        "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
        limiter);
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
	printf("[DEBUG] aborted inside read_line with g_abort: %d\n", g_abort);
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
	if (g_abort)
	{
		printf("[DEBUG] aborted inside here_doc with g_abort: %d\n", g_abort);
		free(res);
		return; // stop heredoc immediately
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
		if (g_abort)
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
