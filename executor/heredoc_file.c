/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:30:07 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/10 14:50:31 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

uintptr_t	ft_get_rnd(t_shell *shell)
{
	uintptr_t	rnd;
	char		*ptr;

	ptr = fts_malloc(shell, 1);
	if (!ptr)
		ft_critical_error(shell);
	rnd = (uintptr_t)ptr;
	free(ptr);
	return (rnd);
}

char	*ft_pseudo_random_filename(t_shell *shell)
{
	char	*fn;
	char	*id;
	size_t	offset;
	size_t	id_len;

	id = fts_itoa(shell, (long)ft_get_rnd(shell));
	id_len = ft_strlen(id);
	fn = fts_malloc(shell, (id_len + 16));
	offset = 0;
	ft_memcpy(fn, "/tmp/minishell-", 15);
	offset += 15;
	ft_memcpy(fn + offset, id, id_len);
	offset += id_len;
	fn[offset] = '\0';
	free(id);
	return (fn);
}

static int	ft_clean_err(char *msg, char *to_free, char *to_unlink, int fd)
{
	if (msg != NULL)
		perror(msg);
	if (to_unlink != NULL)
		unlink(to_unlink);
	if (to_free != NULL)
		free(to_free);
	if (fd >= 0 && close(fd) == -1)
		perror("close");
	return (-1);
}

static int	ft_heredoc_file(t_shell *shell, char *input, size_t input_len)
{
	int		fd;
	char	*path;

	path = ft_pseudo_random_filename(shell);
	fd = open(path, O_CREAT | O_EXCL | O_WRONLY, 0600);
	if (fd < 0)
		return (ft_clean_err("open temp heredoc file for write", \
				path, NULL, fd));
	if (write(fd, input, input_len) != (ssize_t)input_len)
		return (ft_clean_err("write temp heredoc file", \
				path, path, fd));
	close(fd);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ft_clean_err("open temp heredoc file for read", \
				path, path, fd));
	if (unlink(path) < 0)
		return (ft_clean_err("unlink", NULL, path, fd));
	free(path);
	return (fd);
}

int	ft_heredoc_pipe(t_shell *shell, int w, int r, char *input)
{
	size_t	input_len;

	input_len = ft_strlen(input);
	if (input_len <= PIPE_BUF)
	{
		write(w, input, input_len);
		return (r);
	}
	return (ft_heredoc_file(shell, input, input_len));
}
