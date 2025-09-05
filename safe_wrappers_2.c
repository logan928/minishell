/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_wrappers_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:37:37 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:03:16 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*fts_malloc(t_shell *shell, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc");
		ft_critical_error(shell);
	}
	return (ptr);
}

char	*fts_strjoin3cpy(t_shell *shell, char *s1, char *s2, char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*res;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	res = fts_malloc(shell, len1 + len2 + len3 + 1);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	ft_memcpy(res + len1 + len2, s3, len3);
	res[len1 + len2 + len3] = '\0';
	return (res);
}

void	fts_write(t_shell *shell, char *s, int fd)
{
	if (write(fd, s, ft_strlen(s)) != (ssize_t)ft_strlen(s))
	{
		perror("write");
		free(s);
		ft_critical_error(shell);
	}
	free(s);
}
