/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:38:49 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:45:30 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_sort_exp(t_shell *shell)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < shell->exp->len)
	{
		j = 0;
		while (j < shell->exp->len - 1 - i)
		{
			if (ft_strcmp(shell->exp->data[j], shell->exp->data[j + 1], 0) > 0)
			{
				tmp = shell->exp->data[j];
				shell->exp->data[j] = shell->exp->data[j + 1];
				shell->exp->data[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static char	*ft_built_exp_var(t_shell *shell, char *s, char *eq)
{
	char	*res;
	size_t	len;
	size_t	offset;

	len = ft_strlen(s);
	res = fts_malloc(shell, sizeof(char) * (len + 16));
	offset = 0;
	ft_memcpy(res, "declare -x ", 11);
	offset += 11;
	ft_memcpy(res + offset, s, eq - s + 1);
	offset += eq - s + 1;
	ft_memcpy(res + offset, "\"", 1);
	offset++;
	ft_memcpy(res + offset, eq + 1, ft_strlen(eq) - 1);
	offset += ft_strlen(eq) - 1;
	ft_memcpy(res + offset, "\"\n", 2);
	offset += 2;
	res[offset] = '\0';
	return (res);
}

static void	ft_print_exp(t_shell *shell)
{
	char	*res;
	size_t	i;
	char	*eq;
	char	*var;

	ft_sort_exp(shell);
	i = 0;
	res = fts_strdup(shell, "");
	while (shell->exp->data[i] != NULL)
	{
		eq = ft_strchr(shell->exp->data[i], '=');
		if (eq == NULL)
		{
			var = fts_strjoin3cpy(shell,
					"declare -x ", shell->exp->data[i], "\n");
			res = fts_strjoin_free(shell, res, var);
		}
		else
			res = fts_strjoin_free(shell, res,
					ft_built_exp_var(shell, shell->exp->data[i], eq));
		i++;
	}
	fts_write(shell, res, STDOUT_FILENO);
}

static void	ft_set_val(t_shell *sh, char *eq, char *s)
{
	char	*vr;
	char	*vl;
	char	*pr;

	vl = fts_strdup(sh, eq + 1);
	if (*(eq - 1) == '+')
	{
		vr = fts_strndup(sh, s, eq - s - 1);
		pr = fts_strjoin_free(sh, fts_strdup(sh, "="), ft_get_env_var(sh, vr));
		sh->exp = ft_strvec_update(sh->exp,
				vr, fts_strjoin3cpy(sh, vr, pr, vl));
		sh->env = ft_strvec_update(sh->env,
				vr, fts_strjoin3cpy(sh, vr, pr, vl));
		free(pr);
	}
	else
	{
		vr = fts_strndup(sh, s, eq - s);
		sh->exp = ft_strvec_update(sh->exp,
				vr, fts_strjoin3cpy(sh, vr, "=", vl));
		sh->env = ft_strvec_update(sh->env,
				vr, fts_strjoin3cpy(sh, vr, "=", vl));
	}
	free(vl);
	free(vr);
}

void	ft_export(t_shell *shell, char **args)
{
	size_t	i;
	char	*eq;

	if (!args[1])
	{
		ft_print_exp(shell);
		return ;
	}
	i = 1;
	while (args && args[i])
	{
		if (!ft_is_valid_var_name(args[i]))
			ft_not_valid_identifier(shell, args[i]);
		else
		{
			eq = ft_strchr(args[i], '=');
			if (eq == NULL)
				shell->exp = ft_strvec_update(shell->exp,
						args[i], fts_strdup(shell, args[i]));
			else
				ft_set_val(shell, eq, args[i]);
		}
		i++;
	}
}
