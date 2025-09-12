/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:20:06 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/10 13:19:42 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

static int	handle_r_in(t_redir *redir, int shell_type, \
	t_command_kind kind)
{
	int	fd;

	fd = open_file(redir, shell_type, O_RDONLY);
	if (fd < 0)
		return (1);
	if (kind != BUILTIN)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

static int	handle_r_out(t_redir *redir, int shell_type)
{
	int	fd;

	fd = open_file(redir, shell_type, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_r_app(t_redir *redir, int shell_type)
{
	int	fd;

	fd = open_file(redir, shell_type, O_WRONLY | O_CREAT | O_APPEND);
	if (fd < 0)
		return (1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_r_heredoc(t_shell *shell, t_redir *redir, int shell_type, \
	t_command_kind kind)
{
	int	pipefd[2];
	int	heredoc_fd;

	if (pipe(pipefd) == -1) 
	{
		perror("pipe");
		if (shell_type == CHILD_SHELL)
			ft_critical_with_code(shell, 1, NULL, NULL); 
		else
			return (1);
	}
	if (kind != BUILTIN)
	{
		heredoc_fd = ft_heredoc_file(shell, pipefd[1], \
			pipefd[0], redir->file[0]);
		if (heredoc_fd == -1)
		{
			if (shell_type == CHILD_SHELL)
				ft_critical_with_code(shell, 1, NULL, NULL); 
			else
				return (1);
		}
		if (pipefd[1] != -1)
			close(pipefd[1]);
		if (dup2(heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			if (shell_type == CHILD_SHELL)
				ft_critical_with_code(shell, 1, NULL, NULL); 
			else
				return (1);
		}
		close(heredoc_fd);
		close(pipefd[0]);
	}
	return (0);
}

int	handle_redir(t_shell *shell, t_redir *redir, int shell_type, \
	t_command_kind kind )
{
	if (redir->kind == R_IN)
		return (handle_r_in(redir, shell_type, kind));
	else if (redir->kind == R_OUT)
		return (handle_r_out(redir, shell_type));
	else if (redir->kind == R_APP)
		return (handle_r_app(redir, shell_type));
	else if (redir->kind == R_HDOC)
		return (handle_r_heredoc(shell, redir, shell_type, kind));
	return (0);
}
