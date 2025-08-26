/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:36:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/25 17:05:53 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	ft_home_not_set(t_shell *shell, char *cmd)
{
	char	*err;

	err = ft_str_join3_cpy_safe(shell, "minishell: ", cmd, ENOHOME);
	ft_write_safe(shell, err, STDERR_FILENO);
}
