/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:38:49 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/31 20:23:44 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_sort_exp(t_shell *shell)
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

char	*ft_built_exp_var(t_shell *shell, char *s, char *eq)
{
	char	*res;
	size_t	len;
	size_t	offset;

	len = ft_strlen(s);
	res = ft_malloc_safe(shell, sizeof(char) * (len + 16));
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
	res = ft_strdup_safe(shell, "");
	while (shell->exp->data[i] != NULL)
	{
		eq = ft_strchr(shell->exp->data[i], '=');
		if (eq == NULL)
		{
			var = ft_str_join3_cpy_safe(shell, "declare -x ", shell->exp->data[i], "\n");
			res = ft_strjoin_free_safe(shell, res, var);
		}
		else 
			res = ft_strjoin_free_safe(shell, res, 
				ft_built_exp_var(shell, shell->exp->data[i], eq));
		i++;
	}
	ft_write_safe(shell, res, STDOUT_FILENO);
}

void	ft_set_val(t_shell *shell, char *eq, char *s)
{
	char	*var;
	char	*val;
	char	*prev;

	var = ft_strndup_safe(shell, s, eq - s);
	val = ft_strdup_safe(shell, eq);
	if (*(eq - 1) == '+')
	{
		prev = ft_get_env_var(shell, var);
		shell->exp = ft_strvec_update(shell->exp, var, ft_str_join3_cpy_safe(shell, var, prev, val));
		shell->env = ft_strvec_update(shell->env, var, ft_str_join3_cpy_safe(shell, var, prev, val));
		free(val);
		free(var);
		free(prev);
	}
	else
	{
		shell->exp = ft_strvec_update(shell->exp, var, ft_str_join3_cpy_safe(shell, var, val, ""));
		shell->env = ft_strvec_update(shell->env, var, ft_str_join3_cpy_safe(shell, var, val, ""));
		free(val);
		free(var);
	}
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
		{
			char *err = ft_str_join3_cpy_safe(shell, "minishell: export: `", args[i], "': not a valid identifier\n");
			shell->exit_status = 1;
			ft_write_safe(shell, err, STDERR_FILENO);
		}
		else
		{
			eq = ft_strchr(args[i], '=');
			if (eq == NULL)
				shell->exp = ft_strvec_update(shell->exp, args[i], ft_strdup_safe(shell, args[i]));
			else
				ft_set_val(shell, eq, args[i]);
		}
		i++;
	}
}
