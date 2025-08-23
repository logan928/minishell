/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_wrappers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:24:41 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/22 12:37:01 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_safe(t_shell *shell, const char *s)
{
	char	*copy;
	
	copy = ft_strdup(s);
	if (!copy)
		ft_critical_error(shell);
	return (copy);
}

char	*ft_strndup_safe(t_shell *shell, const char *s, size_t n)
{
	char	*copy;

	copy = ft_strndup(s, n);
	if (!copy)
		ft_critical_error(shell);
	return (copy);
}

t_token	*ft_new_token_safe(t_shell *shell, t_token_kind kind, char *data)
{
	t_token	*new;

	new = ft_new_token(kind, data);
	if (!new)
		ft_critical_error(shell);
	return (new);
}

char	*ft_strjoin_free_safe(t_shell *shell, char *s1, char *s2)
{
	char	*new;

	new = ft_strjoin_free(s1, s2);
	if (!new)
		ft_critical_error(shell);
	return (new);
}

char	*ft_itoa_safe(t_shell *shell, long n)
{
	char	*new;

	new = ft_itoa(n);
	if (!new)
		ft_critical_error(shell);
	return (new);
}
