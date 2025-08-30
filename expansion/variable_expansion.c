/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/25 17:14:17 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_valid_env_first_char(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95)
		return (1);
	return (0);
}

int	ft_valid_env_char(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95
		|| (c >= 48 && c <= 57))
		return (1);
	return (0);
}

char	*ft_get_env_var(t_shell *shell, char *s)
{
	char	*var;

	var = ft_strvec_getval(shell->env, s);
	return (ft_strdup_safe(shell, var));
}
/*
char	*ft_get_env_var(t_shell *shell, char *s, size_t len)
{
	size_t	i;

	i = 0;
	while (shell->env->data[i])
	{
		if (!ft_strncmp(s, shell->env->data[i], len))
		{
			if (shell->env->data[i][len] == '=')
				return (ft_strdup_safe(shell, shell->env->data[i] + len + 1));
		}
		i++;
	}
	return (ft_strdup_safe(shell, ""));
}
*/
void	ft_find_next_append(t_shell *s, t_cursor *c, char *t, char **res)
{
	while (t[c->cur])
	{
		if (ft_isquote(t[c->cur]) && c->quote == 0)
			ft_append_unquoted_quote(s, c, t, res);
		else if (ft_isquote(t[c->cur]) && c->quote == t[c->cur])
			ft_append_quoted_quote(s, c, t, res);
		else if ((c->quote != '\'') && t[c->cur] == '$'
			&& ft_valid_env_first_char(t[c->cur + 1]))
			ft_append_variable(s, c, t, res);
		else if ((c->quote != '\'') && t[c->cur] == '$'
			&& t[c->cur + 1] == '?')
			ft_append_exit_status(s, c, res);
		else
			ft_append_normal_chars(s, c, t, res);
	}
}

/*
	* ft_variable_expansion processes each word segment by segment
	* A segment can be one of three types:
	* 	- Unquoted: $ is expanded;
	* 	- Single-quoted: every character is treated literally
	* 	- Double-quoted: $ is expanded
	* #? is expanded to the value of shell->exit_code
	* Single quotes are preserved inside double quotes, and vice versa
*/

void	ft_variable_expansion(t_shell *shell, char **args, size_t idx)
{
	char		*expanded;
	t_cursor	c;

	while (args[idx])
	{
		expanded = NULL;
		c = (t_cursor){0, 0, 0};
		while (args[idx][c.cur])
			ft_find_next_append(shell, &c, args[idx], &expanded);
		free(args[idx]);
		args[idx] = expanded;
		idx++;
	}
}
