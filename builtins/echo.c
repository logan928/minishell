/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:02:54 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/25 13:53:18 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Checks if an argument is a valid -n option.
 * Any number of 'n's following the '-' is considered valid.
*/

static bool	ft_is_n_flag(const char *s, int	*newline)
{
	int	i;

	if (!s || s[0] != '-')
		return (false);
	i = 1;
	while (s[i])
	{
		if (s[i] != 'n')
			return (false);
		i++;
	}
	*newline = 0;
	return (true);
}

/*
 * Newline = 1: Prints a newline at the end.
 * Newline = 0: The -n option is present; no newline is printed.
 *
 * Any number of valid -n options is considered valid
*/

void	ft_echo(t_shell *shell, char **args)
{
	int		first;
	int		i;
	int		newline;
	char	*res;

	res = ft_strdup_safe(shell, "");
	first = 1;
	i = 1;
	newline = 1;
	while (args[i] && ft_is_n_flag(args[i], &newline))
		i++;
	while (args[i])
	{
		if (!first)
			res = ft_strjoin_free_safe(shell, res, ft_strdup_safe(shell, " "));
		first = 0;
		res = ft_strjoin_free_safe(shell, res, ft_strdup_safe(shell, args[i]));
		i++;
	}
	if (newline)
		res = ft_strjoin_free_safe(shell, res, ft_strdup_safe(shell, "\n"));
	ft_write_safe(shell, res, STDOUT_FILENO);
	shell->exit_status = (unsigned char) 0;
}
