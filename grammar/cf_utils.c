/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cf_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:13:41 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/01 17:13:48 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (
		ft_strcmp(cmd, "echo", 0) == 0 
		|| ft_strcmp(cmd, "cd", 0) == 0
		|| ft_strcmp(cmd, "pwd", 0) == 0
		|| ft_strcmp(cmd, "export", 0) == 0
		|| ft_strcmp(cmd, "unset", 0) == 0
		|| ft_strcmp(cmd, "env", 0) == 0
		|| ft_strcmp(cmd, "exit", 0) == 0
	);
}

char	**argv_add(char **argv, int *argc, const char *word)
{
	char	**newv;
	int		i;

	newv = malloc(sizeof(char *) * (*argc + 2));
	if (!newv)
		return (NULL);
	i = 0;
	while (i < *argc)
	{
		newv[i] = argv[i];
		i++;
	}
	newv[i++] = strdup(word);
	newv[i] = NULL;
	free(argv);
	(*argc)++;
	return (newv);
}

t_command	*command_new(void)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->command_kind = EXTERNAL;
	cmd->args = NULL;
	return (cmd);
}
