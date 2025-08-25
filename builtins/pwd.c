/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:58:32 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/25 17:07:43 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_cwd(t_shell *shell)
{
	char	*buf;
	size_t	buf_size;
	char	*res;

	buf_size = 1024;
	while (1)
	{
		buf = ft_malloc_safe(shell, buf_size);
		res = getcwd(buf, buf_size);
		if (res != NULL)
			return (buf);
		free(buf);
		if (errno != ERANGE)
			ft_critical_error(shell);
		buf_size *= 2;
	}
}

void	ft_pwd(t_shell *shell, char **args)
{
	(void)args;
	if (shell->pwd)
		printf("%s\n", shell->pwd);
}
