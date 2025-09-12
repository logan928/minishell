/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:39:04 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/12 15:46:41 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_io_error(t_shell *shell, int shell_type, char *msg)
{
	if (msg != NULL)
		perror(msg);
	if (shell_type == CHILD_SHELL)
		ft_critical_with_code(shell, 1, NULL, NULL); 
	return (1);
}
