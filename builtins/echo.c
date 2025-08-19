/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:02:54 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/19 14:24:23 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo(char **args)
{
	int	first;
	int	n;
	int	i;
	char *res;

	res = NULL;
	first = 1;
	n = 0;
	if (args && !ft_strncmp(args[0], "-n", 2))
		n = 1;
	i = n;
	while (args[i])
	{
		if (!first)
		{
			res = ft_strjoin_free(res, ft_strdup(" "));
			first = 0;
		}
		res = ft_strjoin_free(res, ft_strdup(args[i++]));
	}
	if (!n)
		res = ft_strjoin_free(res, ft_strdup("\n"));
	if (printf("%s", res) >= 0)
		return (0);
	return (-1);
}

/*
char *args[] = {"-n", "Hello world", NULL};
char *args[] = {"Hello world", NULL};
ft_echo(args);
*/
