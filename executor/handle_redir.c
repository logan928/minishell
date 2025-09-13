/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:20:06 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/12 15:47:05 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

static int	handle_r_in(t_shell *shell, t_redir *redir, \
	t_command_kind kind)
{
	int	fd;

	fd = open_file(shell, redir, O_RDONLY);
	if (fd < 0)
		return (1);
	if (kind != BUILTIN)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			return (1);
		}
		close(fd);
	}
	return (0);
}

static int	handle_r_out(t_shell *shell, t_redir *redir)
{
	int	fd;

	fd = open_file(shell, redir, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_r_app(t_shell *shell, t_redir *redir)
{
	int	fd;

	fd = open_file(shell, redir, O_WRONLY | O_CREAT | O_APPEND);
	if (fd < 0)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_r_heredoc(t_shell *shell, t_redir *redir, \
	t_command_kind kind)
{
	int	pipefd[2];
	int	heredoc_fd;

	if (pipe(pipefd) == -1) 
		return (ft_io_error("pipe"));
	if (kind != BUILTIN)
	{
		heredoc_fd = ft_heredoc_pipe(shell, \
			pipefd[1], pipefd[0], redir->file[0]);
		if (heredoc_fd == -1)
			return (1);
		close(pipefd[1]);
		if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			return (ft_io_error("dup2"));
		close(heredoc_fd);
		if (heredoc_fd != pipefd[0])
			close(pipefd[0]);
	}
	return (0);
}

int	handle_redir(t_shell *shell, t_redir *redir, \
	t_command_kind kind )
{
	if (redir->kind == R_IN)
		return (handle_r_in(shell, redir, kind));
	else if (redir->kind == R_OUT)
		return (handle_r_out(shell, redir));
	else if (redir->kind == R_APP)
		return (handle_r_app(shell, redir));
	else if (redir->kind == R_HDOC)
		return (handle_r_heredoc(shell, redir, kind));
	return (0);
}
