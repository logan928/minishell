/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/18 11:13:31 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
	* ft_expand processes each word segment by segment
	* A segment can be one of three types:
	* 	- Unquoted: $ is expanded;
	* 	- Single-quoted: every character is treated literally
	* 	- Double-quoted: $ is expanded
	* #? is expanded to the value of shell->exit_code
	* Single quotes are preserved inside double quotes, and vice versa
*/
void	ft_vars_expansion(t_shell *shell)
{
	t_token	*t;
	char	*res;
	t_lexer *l;

	t = shell->lexer->tokens;
	l = shell->lexer;
	while (t)
	{
		if (t->token_kind == WORD)
		{
			res = ft_strdup("");
			l->pos = 0;
			l->quote = 0;
			l->start = -1;
			while (t->data[l->pos])
			{
				if (ft_isquote(t->data[l->pos]) && l->quote == 0)
				{
					l->quote = t->data[(l->pos)];
					res = ft_strjoin_free(res, ft_strndup(&t->data[l->pos++], 1));
				}
				else if (ft_isquote(t->data[l->pos]) && l->quote == t->data[l->pos])
				{
					res = ft_strjoin_free(res, ft_strndup(&t->data[l->pos++], 1));
					l->quote = 0;
				}
				else if ((l->quote != '\'') && t->data[l->pos] == '$'
					&& ft_valid_env_char(t->data[l->pos + 1]))
				{
					l->start = ++(l->pos);
					while (t->data[l->pos] && ft_valid_env_char(t->data[l->pos]))
						(l->pos)++;
					res = ft_strjoin_free(res, ft_get_env_var(&t->data[l->start], l->pos - l->start, shell->env));
				}
				else if ((l->quote != '\'') && t->data[l->pos] == '$'
					&& t->data[l->pos + 1] == '?')
				{
					res = ft_strjoin_free(res, ft_itoa((long) shell->exit_status));
					l->pos += 2;
				}
				else
				{
					l->start = (l->pos)++;
					while (t->data[l->pos] && !ft_isquote(t->data[l->pos]) && t->data[l->pos] != '$')
						l->pos++;
					res = ft_strjoin_free(res, ft_strndup(&t->data[l->start], l->pos - l->start));
				}
			}
			free(t->data);
			t->data = res;
		}
		t = t->next;
	}
}
