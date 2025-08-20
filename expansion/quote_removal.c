/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:38:46 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/18 10:38:48 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_quote_removal(t_shell *shell)
{
	t_token	*t;
	t_lexer	*l;
	char	*clean;

	t = shell->lexer->tokens;
	l = shell->lexer;
	while (t)
	{
		if (t->token_kind == WORD)
		{
			l->pos = 0;
			l->quote = 0;
			l->start = 0;
			clean = NULL;
			while (t->data[l->pos])
			{
				if (ft_isquote(t->data[l->pos]) && !l->quote)
				{
					l->quote = t->data[l->pos];
					if (l->pos - l->start > 0)
						clean = ft_strjoin_free(clean, ft_strndup(&t->data[l->start], l->pos - l->start));
					l->start = ++l->pos;
				}
				else if (ft_isquote(t->data[l->pos]) && l->quote == t->data[l->pos])
				{
					l->quote = 0;
					if (l->pos - l->start > 0)
						clean = ft_strjoin_free(clean, ft_strndup(&t->data[l->start], l->pos - l->start));
					l->start = ++l->pos;
				}
				else
					l->pos++;
			}
			if (l->pos - l->start > 0)
				clean = ft_strjoin_free(clean, ft_strndup(&t->data[l->start], l->pos - l->start));
			free(t->data);
			if (!clean)
				t->data = ft_strdup("''");
			else
				t->data = clean;
		}
		t = t->next;
	}
}
