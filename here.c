/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:15:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 16:51:16 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_here_doc(t_shell *shell, t_token *t)
{
	char	*line;
	bool	is_quoted;
	char	*res;

	(void)shell;
	is_quoted = ft_is_quoted(t->data);
	if (is_quoted)
		ft_quote_removal_str(shell, t);
	res = NULL;
	while (1)
	{
		write(1, "> ", 2);
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, t->data, ft_strlen(t->data)) == 0
			&& line[ft_strlen(t->data)] == '\n')
		{
			free(line);
			break ;
		}
		res = ft_strjoin_free_safe(shell, res, line);
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
