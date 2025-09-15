/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:15:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:03:07 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_abort = 0;

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

static void	ft_here_eof_warning(t_shell *shell, char *limiter)
{
	char	*err;

	if (!g_abort)
	{
		g_abort = HEREDOC_EOF;
		err = fts_strjoin3cpy(shell,
				"minishell: warning: here-document \
delimited by end-of-file (wanted `",
				limiter,
				"')\n");
		fts_write(shell, err, STDERR_FILENO);
	}
}

static void	ft_read_line(t_shell *shell, char **res, char *limiter)
{
	char	*line;
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	while (1)
	{
		line = readline("> ");
		if (g_abort == HEREDOC_INT)
		{
			free(line);
			break ;
		}
		if (!line)
		{
			ft_here_eof_warning(shell, limiter);
			break ;
		}
		if (ft_strncmp(line, limiter, limiter_len) == 0)
		{
			free(line);
			break ;
		}
		*res = fts_strjoin_free(shell, *res, line);
		*res = fts_strjoin_free(shell, *res, fts_strdup(shell, "\n"));
	}
}

static void	ft_here_doc(t_shell *shell, t_token *t)
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
		res = fts_strdup(shell, "\"\"");
	if (is_quoted)
	{
		res = fts_strjoin_free(shell, fts_strdup(shell, "'"), res);
		res = fts_strjoin_free(shell, res, fts_strdup(shell, "'"));
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
			ft_set_signals_hd_pre();
			ft_here_doc(shell, tmp);
			ft_set_signals_post();
			shell->lexer->io_here = false;
		}
		tmp = tmp->next;
	}
}
