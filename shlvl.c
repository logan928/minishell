/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:21:38 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 18:23:24 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_shlvl_warning(t_shell *sh, char *lvl)
{
	char	*left;
	char	*right;

	left = "minishell: warning: shell sevel (";
	right = ") too high, resetting to 1\n";
	
	ft_write_safe(sh, ft_str_join3_cpy_safe(sh, 
				left, lvl, right), STDERR_FILENO);
}
void	ft_increment_shlvl(t_shell *sh, char *shlvl)
{
	int		lvl;
	char	*end;
	char	*tmp;

	lvl = (int) ft_strtoll(shlvl, &end, 10);
	if (*end != '\0')
		ft_strvec_update(sh->env, "SHLVL", ft_strdup_safe(sh, "SHLVL=1"));
	else
	{
		tmp = ft_itoa_safe(sh, (long)++lvl);
		if (lvl >= 1000)
		{
			ft_shlvl_warning(sh, tmp);
			lvl = 1;
			ft_strvec_update(sh->env, "SHLVL", ft_strdup_safe(sh, "SHLVL=1"));
			free(tmp);
			return ;
		}
		ft_strvec_update(sh->env, "SHLVL", 
			ft_str_join3_cpy_safe(sh, "SHLVL=", tmp, ""));
		free(tmp);
	}
}

void	ft_shlvl(t_shell *shell)
{
	char	*shlvl;

	shlvl = ft_strvec_getval(shell->env, "SHLVL");
	if (shlvl == NULL || ft_strlen(shlvl) == 0)
	{
		if (!ft_strvec_push(&shell->env, ft_strdup_safe(shell, "SHLVL=1")))
			ft_critical_error(shell);
	}
	else if (!ft_is_valid_number(shlvl))
		ft_strvec_update(shell->env, "SHLVL", ft_strdup_safe(shell, "SHLVL=1"));
	else
		ft_increment_shlvl(shell, shlvl);
}
