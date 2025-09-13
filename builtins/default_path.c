/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:37:47 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/13 15:45:24 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_set_default_path(t_shell *shell)
{
	char	*pwd;

	pwd = ft_get_env_var(shell, "PATH");
	if (pwd[0] == '\0')
	{
		free(pwd);
		shell->env = ft_strvec_update(shell->env,
			"PATH", fts_strjoin3cpy(shell, "PATH", "=", DEFAULTPATH));
		shell->exp = ft_strvec_update(shell->exp,
			"PATH", fts_strjoin3cpy(shell, "PATH", "=", DEFAULTPATH));
		return ;
	}
	free(pwd);
}
