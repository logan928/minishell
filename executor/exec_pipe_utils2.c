/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:24:28 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/12 16:24:33 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    handle_check_access(t_shell *shell, t_command *cmd, \
            t_ast **commands, pid_t *pids)
{
    int access_err;

    access_err = ft_check_access(shell, cmd);
	if (access_err)
		ft_critical_with_code(shell, access_err, commands, pids);
}
