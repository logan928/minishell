/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 13:59:10 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/26 15:26:36 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_stack	*ft_stack_init(void)
{
	t_stack	*st;

	st = malloc(sizeof(t_stack));
	if (st == NULL)
		return (NULL);
	st->head = NULL;
	st->size = 0;
	return (st);
}

t_stack	*ft_stack_push(t_stack *st, char *value)
{
	t_char_node	*node;

	if (st == NULL)
		return (NULL);
	node = malloc(sizeof(t_char_node));
	if (node == NULL)
		return (NULL);
	node->value = value;
	node->prev = st->head;
	st->head = node;
	st->size++;
	return (st);
}

char	*ft_stack_pop(t_stack *st)
{
	char		*ret;
	t_char_node	*tmp;
	if (st == NULL || st->size == 0)
		return (NULL);
	ret = st->head->value;
	tmp = st->head;
	st->head = st->head->prev;
	st->size--;
	free(tmp);
	return (ret);
}

void	ft_stack_free(t_stack *st)
{
	t_char_node	*cur;
	t_char_node	*prev;

	if (st == NULL)
		return ;
	if (st->head == NULL)
	{
		free(st);
		return ;
	}
	cur = st->head;
	while (cur)
	{
		prev = cur->prev;
		free(cur);
		st->size--;
		cur = prev;
	}
	free(st);
}

char	*ft_stack_peek(t_stack *st)
{
	if (st == NULL || st->size == 0)
		return (NULL);
	return (st->head->value);
}
