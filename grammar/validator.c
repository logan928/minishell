/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:50:27 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/09 15:03:13 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_isoperator(t_token_kind kind)
{
	return (kind == PIPE || kind == OR_IF);
}

static int	ft_syntax_error(t_lexer *lexer, char *token)
{
	char		*error;

	error = ft_strjoin("minishell: syntax error near unexpected token `",
			token, "'\n");
	if (!error)
	{
		ft_free_lexer(lexer);
		return (-1);
	}
	if (write(2, error, ft_strlen(error)) != (ssize_t)ft_strlen(error))
	{
		printf("test\n");
		free(error);
		ft_free_lexer(lexer);
		return (-1);
	}
	free(error);
	return (0);
}

int	ft_check_syntax(t_lexer *lexer)
{
	t_token	*token;
	t_token	*next;
	int		i;

	i = 0;
	token = lexer->tokens;
	while (token)
	{
		next = token->next;
		if (((i == 0 || next == NULL) && ft_isoperator(token->token_kind))
			|| (ft_isoperator(token->token_kind) && next
				&& ft_isoperator(next->token_kind)))
			return (ft_syntax_error(lexer, token->data));
		i++;
		token = next;
	}
	return (1);
}
