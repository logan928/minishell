/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:36:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/12 13:41:42 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/*()
char	**ft_get_builtins(void)
{
	const char	*builtins[] = {"echo", "cd", "pwd",
		"export", "unset", "env", "exit", NULL};

	return (builtins);
}

int	*ft_is_builtin(const char *builtins[], char *cmd)
{
	size_t	i;

	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(builtins[i], cmd, ft_strlen(cmd)) == 0)
			return (1);
	}
	return (0);
}*/

bool	ft_is_valid_number(char *s)
{
	bool	has_digit;

	has_digit = false;
	while (*s && ft_isspace(*s))
		s++;
	if (*s == '+' || *s == '-')
		s++;
	while (*s && ft_isdigit(*s))
	{
		has_digit = true;
		s++;
	}
	while (*s && ft_isspace(*s))
		s++;
	return (*s == '\0' && has_digit);
}

void	ft_num_arg_req(t_shell *shell, char *cmd, char *arg)
{
	char	*err;

	err = ft_str_join3_cpy_safe(shell, cmd, arg, ENUMREQ);
	ft_write_safe(shell, err, STDERR_FILENO);
}

void	ft_too_many_args(t_shell *shell, char *cmd)
{
	char	*err;

	err = ft_str_join3_cpy_safe(shell, "minishell: ", cmd, ETMARGS);
	ft_write_safe(shell, err, STDERR_FILENO);
}
