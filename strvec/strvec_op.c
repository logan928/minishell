/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strvec_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:06:42 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 17:13:48 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

ssize_t	ft_strvec_find(const t_strvec *sv, const char *s)
{
	size_t	i;
	size_t	s_len;

	s_len = ft_strlen(s);
	i = 0;
	while (i < sv->len)
	{
		if (!ft_strncmp(sv->data[i], s, s_len)
			&& (sv->data[i][s_len] == '\0' || sv->data[i][s_len] == '='))
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_strvec_getval(const t_strvec *sv, const char *s)
{
	ssize_t	idx;
	char	*p;

	idx = ft_strvec_find(sv, s);
	if (idx == -1)
		return ("");
	p = sv->data[idx];
	while (*p)
	{
		if (*p == '=')
			return (p + 1);
		p++;
	}
	return ("");
}

t_strvec	*ft_strvec_push(t_strvec **sv, char *s)
{
	t_strvec	*tmp;

	if (*sv == NULL)
		return (NULL);
	if (!s)
		return (*sv);
	if ((*sv)->len == (*sv)->cap)
	{
		tmp = ft_strvec_realloc(*sv);
		if (tmp == NULL)
			return (NULL);
		*sv = tmp;
	}
	(*sv)->data[(*sv)->len] = s;
	(*sv)->len++;
	(*sv)->data[(*sv)->len] = NULL;
	return (*sv);
}

size_t	ft_strvec_remove(t_strvec *sv, const char *s)
{
	ssize_t	idx;

	if (sv == NULL || sv->len == 0)
		return (0);
	idx = ft_strvec_find(sv, s);
	if (idx == -1)
		return (0);
	free(sv->data[idx]);
	while ((size_t)idx < sv->len - 1)
	{
		sv->data[idx] = sv->data[idx + 1];
		idx++;
	}
	sv->len--;
	sv->data[sv->len] = NULL;
	return (1);
}

t_strvec	*ft_strvec_update(t_strvec *sv, char *s, char *val)
{
	ssize_t	idx;

	if (sv == NULL)
		return (NULL);
	idx = ft_strvec_find(sv, s);
	if (idx == -1)
	{
		sv = ft_strvec_push(&sv, val);
		if (sv == NULL)
			return (NULL);
	}
	else
	{
		free(sv->data[idx]);
		sv->data[idx] = val;
	}
	return (sv);
}
