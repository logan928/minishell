/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:50:27 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/07 18:18:38 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_isoperator(t_token_kind kind)
{
	return (kind == PIPE || kind == OR_IF);
}

void	ft_check_syntax(t_lexer *lexer)
{
	t_token	*token;
	t_token	*next;
	int		i;

	i = 0;
	token = lexer->tokens;
	while (token)
	{
		next = token->next;
		if ((i == 0 || next == NULL)
			&& ft_isoperator(token->token_kind))
		{
			printf("minishell: syntax error near unexpected token `%s`\n", token->data);
			ft_free_lexer(lexer);
			return ;
		}
		if ((ft_isoperator(token->token_kind) && next && ft_isoperator(next->token_kind)))
		{
			printf("minishell: syntax error near unexpected token `%s`\n", next->data);
			ft_free_lexer(lexer);
			return ;
		}
		i++;
		token = next;
	}
	return ;
}
