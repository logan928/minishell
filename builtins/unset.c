#include "../minishell.h"

void	ft_unset(t_shell *shell, char **args)
{
	size_t	i;
	size_t	j;
	size_t	len_var;

	if (!args && !args[0] && !args[1])
		return ;
	j = 1;
	while (args[j] != NULL)
	{
		i = 0;
		while (shell->env[i])
		{
			len_var = ft_strlen(args[j]);
			if (!ft_strncmp(args[j], shell->env[i], len_var))
			{
				if (shell->env[i][len_var] == '=')
					ft_remove_at(shell->env, i);
				continue ;
			}
			i++;
		}
		j++;
	}
}
