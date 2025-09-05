/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_wrappers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:24:41 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 14:58:17 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*fts_strdup(t_shell *shell, const char *s)
{
	char	*copy;

	copy = ft_strdup(s);
	if (!copy)
	{
		perror("ft_strdup");
		ft_critical_error(shell);
	}
	return (copy);
}

char	*fts_strndup(t_shell *shell, const char *s, size_t n)
{
	char	*copy;

	copy = ft_strndup(s, n);
	if (!copy)
	{
		perror("ft_strndup");
		ft_critical_error(shell);
	}
	return (copy);
}

t_token	*fts_new_token(t_shell *shell, t_token_kind kind, char *data)
{
	t_token	*new;

	new = ft_new_token(kind, data);
	if (!new)
	{
		perror("ft_new_token");
		ft_critical_error(shell);
	}
	return (new);
}

char	*fts_strjoin_free(t_shell *shell, char *s1, char *s2)
{
	char	*new;

	new = ft_strjoin_free(s1, s2);
	if (!new)
	{
		perror("ft_strjoin_free");
		ft_critical_error(shell);
	}
	return (new);
}

char	*fts_itoa(t_shell *shell, long n)
{
	char	*new;

	new = ft_itoa(n);
	if (!new)
	{
		perror("ft_itoa");
		ft_critical_error(shell);
	}
	return (new);
}
