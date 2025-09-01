/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strvec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:13:29 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 17:13:30 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_strvec	*ft_strvec_init(size_t cap)
{
	t_strvec	*new;

	if (cap == 0)
		cap = 1;
	new = ft_calloc(1, sizeof(t_strvec));
	if (new == NULL)
		return (NULL);
	new->len = 0;
	new->cap = cap;
	new->data = ft_calloc(cap + 1, sizeof(char *));
	if (new->data == NULL)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

void	ft_strvec_free(t_strvec *sv)
{
	size_t	i;

	i = 0;
	while (i < sv->len)
		free(sv->data[i++]);
	free(sv->data);
	free(sv);
}

t_strvec	*ft_strvec_realloc(t_strvec *sv)
{
	t_strvec	*new;
	size_t		i;

	new = ft_strvec_init(sv->cap * 2);
	if (!new)
		return (NULL);
	new->len = sv->len;
	i = 0;
	while (i < sv->len)
	{
		new->data[i] = sv->data[i];
		i++;
	}
	new->data[i] = NULL;
	free(sv->data);
	free(sv);
	return (new);
}
