/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:36:35 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/12 13:41:42 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	**ft_get_builtins(void)
{
	const char	*builtins[] = {"echo", "cd", "pwd",
		"export", "unset", "env", "exit", NULL};

	return (builtins);
}

int	*ft_is_builtin(const char *builtins[], char *cmd)
{
	size_t	i;

	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(builtins[i], cmd, ft_strlen(cmd)) == 0)
			return (1);
	}
	return (0);
}
