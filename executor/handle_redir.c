/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:20:06 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/03 17:20:09 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

static int	handle_r_in(t_redir *redir, int shell_type, int flags, \
	t_command_kind kind)
{
	int	fd;

	fd = open_file(redir, shell_type, flags);
	if (fd < 0)
		return (1);
	if (kind != BUILTIN)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

static int	handle_r_out(t_redir *redir, int shell_type, int flags)
{
	int	fd;

	fd = open_file(redir, shell_type, flags);
	if (fd < 0)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_r_app(t_redir *redir, int shell_type, int flags)
{
	int	fd;

	fd = open_file(redir, shell_type, flags);
	if (fd < 0)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_r_heredoc(t_redir *redir, int shell_type, \
	t_command_kind kind)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1) 
	{
		perror("pipe");
		if (shell_type == CHILD_SHELL)
			exit(1); 
		else
			return (1);
	}
	if (kind != BUILTIN)
	{
		write(pipefd[1], redir->file[0], strlen(redir->file[0]));
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	return (0);
}

int	handle_redir(t_redir *redir, int shell_type, \
	t_command_kind kind )
{
	if (redir->kind == R_IN)
		return (handle_r_in(redir, shell_type, O_RDONLY, kind));
	else if (redir->kind == R_OUT)
		return (handle_r_out(redir, shell_type, O_WRONLY | O_CREAT | O_TRUNC));
	else if (redir->kind == R_APP)
		return (handle_r_app(redir, shell_type, O_WRONLY | O_CREAT | O_APPEND));
	else if (redir->kind == R_HDOC)
		return (handle_r_heredoc(redir, shell_type, kind));
	return (0);
}
