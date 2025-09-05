/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:44:24 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/05 15:40:32 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int write_safe_return_wrapper(t_shell *shell, t_command *cmd,
	t_cehck_access_msgs acc_para, int exit_condition)
{
	char			*cmd_name;

	cmd_name = fts_strdup(shell, cmd->args[0]);
	if (acc_para.is_access_exists)
	{
		fts_write(shell,
			fts_strjoin3cpy(shell, "minishell: ", cmd->args[0], \
			acc_para.msg), STDERR_FILENO);
	}
	else
	{
		fts_write(shell,
			fts_strjoin3cpy(shell, cmd->args[0], \
			acc_para.msg, ""), STDERR_FILENO);
	}
	free(cmd_name);
	return (exit_condition);
}

static int	handle_access_exist(t_shell *shell, t_command *cmd, t_cehck_access_msgs acc_para, t_cmd_access access)
{
	if (ft_strchr(cmd->args[0], '/') != NULL || !access.path
		|| (cmd->args[0][0] && !access.path))
	{
		acc_para.msg = ": No such file or directory\n";
		acc_para.is_access_exists = true;
		return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_FOUND));
	}
	else
	{
		acc_para.msg = ": command not found\n";
		acc_para.is_access_exists = false;
		return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_FOUND));
	}
}

int	ft_check_access(t_shell *shell, t_command *cmd)
{
	char				*cmd_name;
	t_cmd_access		access;
	t_cehck_access_msgs	acc_para;

	cmd_name = fts_strdup(shell, cmd->args[0]);
	access = ft_get_cmd_path(shell, cmd->args);
	acc_para = (t_cehck_access_msgs){NULL, true};
	if (!access.exist || (access.is_dir && ft_strchr(cmd->args[0], '/') == NULL && access.path))
		return (handle_access_exist(shell, cmd, acc_para, access));
	else if (access.is_dir)
	{
		acc_para.msg = ": Is a directory\n";
		acc_para.is_access_exists = true;
		return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_EXEC));
	}
	else if (!access.executable)
	{
		acc_para.msg = ": Permission denied\n";
		acc_para.is_access_exists = true;
		return (write_safe_return_wrapper(shell, cmd, acc_para, CMD_NOT_EXEC));
	}
	free(cmd_name);
	return (0);
}
