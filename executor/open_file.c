/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:15:05 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/04 01:02:07 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

//TODO: SHELL
int	open_file(t_redir *redir, int shell_type, int flags)
{
	int	fd;

	fd = open(redir->file[0], flags, 0644);
	if (fd < 0)
	{
		perror("open");
		if (shell_type == CHILD_SHELL)
			exit(1);
		else
			return (-1);
	}
	return (fd);
}
