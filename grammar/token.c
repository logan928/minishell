/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:32:53 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 17:40:49 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_new_token(t_token_kind kind, char *data)
{
	t_token	*token;

	if (!data)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
	{
		if (data)
			free(data);
		return (NULL);
	}
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

void	ft_free_tokens(t_token *head)
{
	t_token	*cur;
	t_token	*next;

	cur = head;
	while (cur)
	{
		next = cur->next;
		free(cur->data);
		free(cur);
		cur = next;
	}
}

void	ft_insert_after(t_token *target, t_token *token)
{
	token->next = target->next;
	target->next = token;
}


void	ft_free_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	if (lexer->word)
	{
		free(lexer->word);
		lexer->word = NULL;
	}
	if (lexer->tokens)
	{
		ft_free_tokens(lexer->tokens);
		lexer->tokens = NULL;
	}
	if (lexer->split_tmp)
	{
		ft_free_tokens(lexer->split_tmp);
		lexer->split_tmp = NULL;
	}
}

void ft_reset_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	ft_free_lexer(lexer);
    lexer->pos = 0;
    lexer->start = 0;
    lexer->quote = 0;
}

void ft_reset_lexer_cursor(t_lexer *lexer)
{
	if (!lexer)
		return ;
    lexer->pos = 0;
    lexer->start = 0;
    lexer->quote = 0;
}
