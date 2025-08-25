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

/*
 *
 * On shell startup:
If: PWD is unset, set it to getcwd()
Else If: PWD is set, but the directory described by PWD does not exist, set it to getcwd()
Else If: PWD is set, but the directory described by PWD does not have the same inode number as the directory described by getcwd(), then set it to getcwd()
Else: don't change it
Set a hidden variable to the value of PWD. It cannot be set or unset manually, only cd can change it
When calling cd:
Set PWD to the path requested (if chdir() was successful), instead of the value of getcwd()
On success, set the hidden variable to PWD
When calling pwd:
Print the hidden variable, ignore the value of PWD or getcwd().
*/
void	ft_pwd(void)
{

}
