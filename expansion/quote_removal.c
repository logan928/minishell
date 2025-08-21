/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:46 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 17:46:12 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_append_before_open_quote(t_shell *shell, char *s, char **clean)
{
	int		start;
	int		pos;
	char	*tmp;

	pos = shell->lexer->pos;
	start = shell->lexer->start;
	shell->lexer->quote = s[pos];
	if (pos - start > 0)
	{
		tmp = ft_strndup_safe(shell, &s[start], pos - start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	shell->lexer->pos++;
	shell->lexer->start = shell->lexer->pos;
}

void	ft_append_before_close_quote(t_shell *shell, char *s, char **clean)
{
	int		start;
	int		pos;
	char	*tmp;

	pos = shell->lexer->pos;
	start = shell->lexer->start;
	shell->lexer->quote = 0;
	if (pos - start > 0)
	{
		tmp = ft_strndup_safe(shell, &s[start], pos - start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	shell->lexer->pos++;
	shell->lexer->start = shell->lexer->pos;
}

void	ft_append_rest(t_shell *shell, char **s, char **clean)
{
	int		start;
	int		pos;
	char	*tmp;

	pos = shell->lexer->pos;
	start = shell->lexer->start;
	if (pos - start > 0)
	{
		tmp = ft_strndup_safe(shell, &(*s)[start], pos - start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	if (*s)
		free(*s);
	if (!*clean)
		*s = ft_strdup_safe(shell, "''");
	else
		*s = *clean;
}

void	ft_quote_removal(t_shell *shell, char **args)
{
	char	*clean;
	size_t	i;

	i = 1;
	while (args[i])
	{
		ft_reset_lexer_cursor(shell->lexer);
		clean = NULL;
		while (args[i][shell->lexer->pos])
		{
			if (ft_isquote(args[i][shell->lexer->pos])
				&& !shell->lexer->quote)
				ft_append_before_open_quote(shell, args[i], &clean);
			else if (ft_isquote(args[i][shell->lexer->pos])
				&& shell->lexer->quote == args[i][shell->lexer->pos])
				ft_append_before_close_quote(shell, args[i], &clean);
			else
				shell->lexer->pos++;
		}
		ft_append_rest(shell, &args[i], &clean);
		i++;
	}
}
