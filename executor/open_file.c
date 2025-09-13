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

int	open_file(t_shell *shell, t_redir *redir, int shell_type, int flags)
{
	int		fd;
	char	*msg;
	
	//(void)shell;
	(void)shell_type;
	fd = open(redir->file[0], flags, 0644);
	if (fd < 0)
	{
		msg = fts_strjoin3cpy(shell, "minishell: ", redir->file[0], "");
		perror(msg);
		free(msg);
		shell->exit_status = 1;
		return (-1);
	}
	return (fd);
}
