/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_append.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:02:40 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 15:04:39 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_append_unquoted_quote(t_shell *s, char *t, char **res)
{
	char	*quote;

	s->lexer->quote = t[s->lexer->pos];
	quote = ft_strndup_safe(s, &t[s->lexer->pos], 1);
	*res = ft_strjoin_free_safe(s, *res, quote);
	s->lexer->pos++;
}

void	ft_append_quoted_quote(t_shell *s, char *t, char **res)
{
	char	*quote;

	s->lexer->quote = 0;
	quote = ft_strndup_safe(s, &t[s->lexer->pos], 1);
	*res = ft_strjoin_free_safe(s, *res, quote);
	s->lexer->pos++;
}

void	ft_append_variable(t_shell *s, char *t, char **res)
{
	char	*var;
	int		start;
	int		len;

	s->lexer->start = ++s->lexer->pos;
	while (t[s->lexer->pos] && ft_valid_env_char(t[s->lexer->pos]))
		s->lexer->pos++;
	start = s->lexer->start;
	len = s->lexer->pos - start;
	var = ft_get_env_var(s, &t[start], len, s->env);
	*res = ft_strjoin_free_safe(s, *res, var);
}

void	ft_append_exit_status(t_shell *s, char **res)
{
	char	*val;

	val = ft_itoa_safe(s, (long) s->exit_status);
	*res = ft_strjoin_free_safe(s, *res, val);
	s->lexer->pos += 2;
}

void	ft_append_normal_chars(t_shell *s, char *t, char **res)
{
	char	*str;
	int		start;
	int		len;

	s->lexer->start = s->lexer->pos++;
	while (t[s->lexer->pos]
		&& !ft_isquote(t[s->lexer->pos])
		&& t[s->lexer->pos] != '$')
		s->lexer->pos++;
	start = s->lexer->start;
	len = s->lexer->pos - start;
	str = ft_strndup_safe(s, &t[s->lexer->start], len);
	*res = ft_strjoin_free_safe(s, *res, str);
}
