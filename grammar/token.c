/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:32:53 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/06 13:43:25 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_new_token(t_token_kind kind, char *data)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->token_kind = kind;
	token->data = data;
	token->next = NULL;
	return (token);
}

void	ft_add_token(t_token **head, t_token *token)
{
	t_token	*cur;

	if (!*head)
		*head = token;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = token;
	}
}
