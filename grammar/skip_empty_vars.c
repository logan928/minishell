/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_empty_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:10:49 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 17:16:40 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool ft_is_valid_plain_var_name(char *s)
{
	size_t	i;

	if (s == NULL || s[0] == '\0' || s[0] != '$')
		return (false);
	if (!ft_valid_env_first_char(s[1]))
		return (false);
	i = 2;
	while (s[i])
	{
		if (!ft_valid_env_char(s[i]))
			return (false);
		i++;
	}
	return (true);
}

void	ft_remove_empty_word(t_shell *shell, t_token *t, t_token *p)
{
	if (p == NULL)
	{
		shell->lexer->tokens = t->next;
		free(t->data);
		free(t);
		t = shell->lexer->tokens;
	}
	else
	{
		p->next = t->next;
		free(t->data);
		free(t);
	}
}

void	ft_skip_empty_vars(t_shell *shell)
{
	t_token	*t;
	t_token *prev;

	t = shell->lexer->tokens;
	prev = NULL;
	while (t)
	{
		if (t->token_kind == WORD && ft_is_valid_plain_var_name(t->data)
			&& ft_strvec_find(shell->env, t->data + 1) == - 1)
			ft_remove_empty_word(shell, t, prev);
		prev = t;
		t = t->next;
	}
}
