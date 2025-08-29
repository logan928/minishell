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

void	ft_append_before(t_shell *shell, t_cursor *c, char *s, char **clean)
{
	char	*tmp;

	c->quote = s[c->cur];
	if (c->cur - c->start > 0)
	{
		tmp = ft_strndup_safe(shell, &s[c->start], c->cur - c->start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	c->cur++;
	c->start = c->cur;
}

void	ft_append_between(t_shell *shell, t_cursor *c, char *s, char **clean)
{
	char	*tmp;

	c->quote = 0;
	if (c->cur - c->start > 0)
	{
		tmp = ft_strndup_safe(shell, &s[c->start], c->cur - c->start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	c->cur++;
	c->start = c->cur;
}

void	ft_append_rest(t_shell *shell, t_cursor *c, char **s, char **clean)
{
	char	*tmp;

	if (c->cur - c->start > 0)
	{
		tmp = ft_strndup_safe(shell, &(*s)[c->start], c->cur - c->start);
		*clean = ft_strjoin_free_safe(shell, *clean, tmp);
	}
	if (*s)
		free(*s);
	if (!*clean)
		*s = ft_strdup_safe(shell, "");
	else
		*s = *clean;
}

void	ft_quote_removal_str(t_shell *shell, t_token *t)
{
	char		*clean;
	t_cursor	c;

	c = (t_cursor){0, 0, 0};
	clean = NULL;
	while (t->data[c.cur])
	{
		if (ft_isquote(t->data[c.cur])
			&& !c.quote)
			ft_append_before(shell, &c, t->data, &clean);
		else if (ft_isquote(t->data[c.cur])
			&& c.quote == t->data[c.cur])
			ft_append_between(shell, &c, t->data, &clean);
		else
			c.cur++;
	}
	ft_append_rest(shell, &c, &t->data, &clean);
}

void	ft_quote_removal(t_shell *shell, char **args, size_t idx)
{
	char		*clean;
	t_cursor	c;

	c = (t_cursor){0, 0, 0};
	while (args[idx])
	{
		clean = NULL;
		while (args[idx][c.cur])
		{
			if (ft_isquote(args[idx][c.cur])
				&& !c.quote)
				ft_append_before(shell, &c, args[idx], &clean);
			else if (ft_isquote(args[idx][c.cur])
				&& c.quote == args[idx][c.cur])
				ft_append_between(shell, &c, args[idx], &clean);
			else
				c.cur++;
		}
		ft_append_rest(shell, &c, &args[idx], &clean);
		idx++;
	}
}
