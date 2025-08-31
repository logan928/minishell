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

bool ft_is_valid_var_name(char *s)
{
	size_t	i;

	if (s == NULL || s[0] == '\0')
		return (false);
	if (!ft_valid_env_first_char(s[0]))
		return (false);
	i = 1;
	while (s[i])
	{
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

void	ft_home_not_set(t_shell *shell, char *cmd, char *tmp)
{
	char	*err;

	free(tmp);
	shell->exit_status = (unsigned char) 1;
	err = ft_str_join3_cpy_safe(shell, "minishell: ", cmd, ENOHOME);
	ft_write_safe(shell, err, STDERR_FILENO);
}
