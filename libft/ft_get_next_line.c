/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:43:19 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/27 13:14:53 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_str_append_mem(char **s1, char *s2, size_t size2)
{
	size_t	size1;
	char	*tmp;

	if (*s1)
		size1 = ft_strlen(*s1);
	else
		size1 = 0;
	tmp = malloc(size2 + size1 + 1);
	if (!tmp)
		return (0);
	ft_memcpy(tmp, *s1, size1);
	ft_memcpy(tmp + size1, s2, size2);
	tmp[size1 + size2] = '\0';
	free(*s1);
	*s1 = tmp;
	return (1);
}

static char	*ft_append_return(char **tmp, char **ret, char *b)
{
	size_t	line_len;
	size_t	leftover_len;

	line_len = *tmp - b + 1;
	if (!ft_str_append_mem(ret, b, line_len))
	{
		free(*ret);
		return (NULL);
	}
	leftover_len = ft_strlen(*tmp + 1);
	ft_memmove(b, *tmp + 1, leftover_len + 1);
	return (*ret);
}

static char	*ft_free_ret(char **ret, ssize_t read_ret)
{
	if (read_ret == -1)
	{
		free(*ret);
		return (NULL);
	}
	else
	{
		if (*ret && **ret)
			return (*ret);
		else
		{
			free(*ret);
			return (NULL);
		}
	}
}

char	*ft_get_next_line(int fd)
{
	static char	b[BUFFER_SIZE + 1] = "";
	char		*ret;
	char		*tmp;
	ssize_t		read_ret;

	ret = NULL;
	while (1)
	{
		tmp = ft_strchr(b, '\n');
		if (tmp)
			return (ft_append_return(&tmp, &ret, b));
		if (!ft_str_append_mem(&ret, b, ft_strlen(b)))
			return (ft_free_ret(&ret, -1));
		read_ret = read(fd, b, BUFFER_SIZE);
		if (read_ret == -1)
			return (ft_free_ret(&ret, read_ret));
		b[read_ret] = 0;
		if (read_ret == 0)
			return (ft_free_ret(&ret, read_ret));
	}
	return (ret);
}
