/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 15:04:51 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_append_unquoted_quote_arr(t_shell *s, char *t, char **res);
void	ft_append_quoted_quote_arr(t_shell *s, char *t, char **res);
void	ft_append_variable_arr(t_shell *s, char *t, char **res);
void	ft_append_normal_chars_arr(t_shell *s, char *t, char **res);

int	ft_valid_env_char_arr(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c == 95))
		return (1);
	return (0);
}

char	*ft_get_env_var_arr(t_shell *shell, char *s, size_t len, char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(s, env[i], len))
		{
			if (env[i][len] == '=')
				return (ft_strdup_safe(shell, env[i] + len + 1));
		}
		env++;
	}
	return (ft_strdup_safe(shell, ""));
}

void	ft_find_next_append_arr(t_shell *s, t_lexer *l, char *t, char **res)
{
	while (t[l->pos])
	{
		if (ft_isquote(t[l->pos]) && l->quote == 0)
			ft_append_unquoted_quote_arr(s, t, res);
		else if (ft_isquote(t[l->pos]) && l->quote == t[l->pos])
			ft_append_quoted_quote_arr(s, t, res);
		else if ((l->quote != '\'') && t[l->pos] == '$'
			&& ft_valid_env_char_arr(t[l->pos + 1]))
			ft_append_variable_arr(s, t, res);
		else if ((l->quote != '\'') && t[l->pos] == '$'
			&& t[l->pos + 1] == '?')
			ft_append_exit_status(s, res);
		else
			ft_append_normal_chars_arr(s, t, res);
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
void	ft_variable_expansion_arr(t_shell *shell, char **args)
{
	size_t	i;
	char	*expanded;

	i = 1;
	
	while (args[i])
	{
		expanded = ft_strdup_safe(shell, "");
		ft_reset_lexer_cursor(shell->lexer);
		while (args[i][shell->lexer->pos])
			ft_find_next_append_arr(shell, shell->lexer, args[i], &expanded);
		free(args[i]);
		args[i] = expanded;
		i++;
	}
}
