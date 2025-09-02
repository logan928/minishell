/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:36:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/31 19:50:03 by mkugan           ###   ########.fr       */
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

bool	ft_is_valid_var_name(char *s)
{
	size_t	i;

	if (s == NULL || s[0] == '\0')
		return (false);
	if (!ft_valid_env_first_char(s[0]))
		return (false);
	i = 1;
	while (s[i])
	{
		if (s[i] == '+')
			return (s[i + 1] == '=');
		if (s[i] == '=')
			return (true);
		if (!ft_valid_env_char(s[i]))
			return (false);
		i++;
	}
	return (true);
}

void	ft_too_many_args(t_shell *shell, char *cmd, unsigned char exit)
{
	char	*err;

	shell->exit_status = exit;
	err = ft_str_join3_cpy_safe(shell, "minishell: ", cmd, ETMARGS);
	ft_write_safe(shell, err, STDERR_FILENO);
}

void	ft_not_set(t_shell *shell, char *var)
{
	char	*err;

	shell->exit_status = (unsigned char) 1;
	err = ft_str_join3_cpy_safe(shell, "minishell: cd: ", var, " not set\n");
	ft_write_safe(shell, err, STDERR_FILENO);
}

void	ft_not_valid_identifier(t_shell *shell, char *arg)
{
	char	*err;

	err = ft_str_join3_cpy_safe(shell, "minishell: export: `",
			arg, "': not a valid identifier\n");
	shell->exit_status = 1;
	ft_write_safe(shell, err, STDERR_FILENO);
}
