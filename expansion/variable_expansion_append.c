/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_append.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:02:40 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/29 16:38:50 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_app_uquote(t_shell *s, t_cursor *c, char *t, char **res)
{
	char	*quote;

	c->quote = t[c->cur];
	quote = ft_strndup_safe(s, &t[c->cur], 1);
	*res = ft_strjoin_free_safe(s, *res, quote);
	c->cur++;
}

void	ft_app_qquote(t_shell *s, t_cursor *c, char *t, char **res)
{
	char	*quote;

	c->quote = 0;
	quote = ft_strndup_safe(s, &t[c->cur], 1);
	*res = ft_strjoin_free_safe(s, *res, quote);
	c->cur++;
}

void	ft_app_var(t_shell *s, t_cursor *c, char *t, char **res)
{
	char	*var;
	int		start;
	char	*search;

	c->start = ++c->cur;
	while (t[c->cur] && ft_valid_env_char(t[c->cur]))
		c->cur++;
	start = c->start;
	search = ft_strndup_safe(s, &t[start], c->cur - start);
	var = ft_get_env_var(s, search);
	free(search);
	*res = ft_strjoin_free_safe(s, *res, var);
}

void	ft_app_exit(t_shell *s, t_cursor *c, char **res)
{
	char	*val;

	val = ft_itoa_safe(s, (long) s->exit_status);
	*res = ft_strjoin_free_safe(s, *res, val);
	c->cur += 2;
}

void	ft_app_char(t_shell *s, t_cursor *c, char *t, char **res)
{
	char	*str;
	int		start;
	int		len;

	c->start = c->cur++;
	while (t[c->cur] && !ft_isquote(t[c->cur]) && t[c->cur] != '$')
		c->cur++;
	start = c->start;
	len = c->cur - start;
	str = ft_strndup_safe(s, &t[c->start], len);
	*res = ft_strjoin_free_safe(s, *res, str);
}
