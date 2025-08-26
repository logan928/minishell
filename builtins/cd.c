/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:56:56 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/26 16:50:02 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	1. If args[1] == NULL and no $HOME - implementation defined
	2. If args[1] == NULL and $HOME is set, try to use $HOME as dir
	3. curpath = args[1]
	4. if curpath isn't absolute, set it to pwd + '/' + args[1]
*/

#include "../minishell.h"

char	*ft_reconstruct_path(t_stack *st)
{
	size_t	size;
	char	**parts;
	size_t	i;
	size_t	len;
	char	*res;

	if (st == NULL || st->size == 0)
		return (ft_strdup("/"));
	size = st->size;
	parts = malloc(sizeof(char *) * size);
	if (!parts)
		return (NULL);
	i = 0;
	while (i < size)
		parts[i++] = ft_stack_pop(st);
	len = 1;
	i = 0;
	while (i < size)
		len += ft_strlen(parts[i++]);
	len += size;
	res = malloc(len);
	if (!res)
	{
		ft_free_arr(parts);
		return (NULL);
	}
	res[0] = '/';
	res[1] = '\0';
	i = size;
	while (i > 0)
	{
		ft_strlcat(res, parts[i - 1], len);
		if (i > 1)
            ft_strlcat(res, "/", len);
        i--;
	}
	free(parts);
	return (res);
}

char	*ft_canonicalize(t_shell *shell, char *curpath)
{
	t_stack	*st;
	char	**segments;
	size_t	i;
	char	*res;

	st = ft_stack_init();
	if (st == NULL)
	{
		free(curpath);
		ft_critical_error(shell);
	}
	segments = ft_split(curpath, '/');
	if (!segments)
	{
		free(curpath);
		ft_stack_free(st);
	}
	i = 0;
	while (segments[i] != NULL)
	{
		if (ft_strcmp(segments[i], ".", 0) == 0)
		{
			i++;
			continue ;
		}
		else if (ft_strcmp(segments[i], "..", 0) == 0)
		{
			if (ft_stack_peek(st) != NULL)
				ft_stack_pop(st);
		}
		else {
			if (ft_stack_push(st, segments[i]) == NULL)
			{
				free(curpath);
				ft_stack_free(st);
				ft_free_arr(segments);
				ft_critical_error(shell);
			}
		}
		i++;
	}
	res = ft_reconstruct_path(st);
	free(curpath);
	ft_stack_free(st);
	free(segments);
	return (res);
}

void	ft_set_env_var(t_shell *shell, char *var, char *val)
{
	size_t	i;
	size_t	len_var;

	i = 0;
	len_var = ft_strlen(var);
	while (shell->env[i])
	{
		if (!ft_strncmp(var, shell->env[i], len_var))
		{
			if (shell->env[i][len_var] == '=')
			{
				free(shell->env[i]);
				shell->env[i] = ft_str_join3_cpy_safe(shell, var, "=", val);
				return ;
			}
		}
		i++;
	}
}

void	ft_chdir(t_shell *shell, char *path)
{
	if (chdir(path) == 0)
	{
		ft_set_env_var(shell, "PWD", path);
		free(shell->pwd);
		shell->pwd = ft_get_env_var(shell, "PWD", 3);
	}
	else
		perror("minishell: cd");
}

void	ft_cd(t_shell *shell, char **args)
{
	char	*curpath;
	char	*tmp;

	curpath = NULL;
	if (args[1] != NULL && args[2] != NULL)
	{
		ft_too_many_args(shell, "cd");
		shell->exit_status = (unsigned char) 1;
		return ;
	}
	if (args[1] == NULL)
	{
		tmp = ft_get_env_var(shell, "HOME", 4);
		if (tmp[0] == '\0')
		{
			ft_home_not_set(shell, "cd");
			free(tmp);
			shell->exit_status = (unsigned char) 1;
			return ;
		}
		curpath = tmp;
		tmp = NULL;
	}
	else
	{
		if (args[1][0] != '/')
			curpath = ft_str_join3_cpy_safe(shell, ft_get_pwd(shell), "/", args[1]);
		else
			curpath = ft_strdup_safe(shell, args[1]);
	}
	curpath = ft_canonicalize(shell, curpath);
	ft_chdir(shell, curpath);
}
