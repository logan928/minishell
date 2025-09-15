/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:59:48 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:11:43 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_exit(t_shell *shell)
{
	ft_free_lexer(shell->lexer);
	if (shell->prompt)
		free(shell->prompt);
	ft_strvec_free(shell->exp);
	ft_strvec_free(shell->env);
	if (shell->pwd)
		free(shell->pwd);
	rl_clear_history();
	exit(shell->exit_status);
}

static void	ft_num_arg_req(t_shell *shell, char *cmd, char *arg)
{
	char	*err;

	shell->exit_status = (unsigned char) 2;
	err = fts_strjoin3cpy(shell, cmd, arg, ENUMREQ);
	fts_write(shell, err, STDERR_FILENO);
	ft_free_exit(shell);
}

static void	ft_check_arg(t_shell *shell, char *arg)
{
	char		*end;
	long long	n;

	if (arg[0] == '-' && arg[1] == '-' && arg[2] == '\0')
		n = 0;
	else if (!ft_is_valid_number(arg))
		return (ft_num_arg_req(shell, "minishell: exit: ", arg));
	else
	{
		n = ft_strtoll(arg, &end, 10);
		if (*end != '\0')
		{
			while (ft_isspace(*end))
				end++;
			if (*end != '\0')
				return (ft_num_arg_req(shell, "minishell: exit: ", arg));
		}
		shell->exit_status = (unsigned char) n;
	}
}

void	ft_exit(t_shell *shell, char **args, int shell_type, pid_t fd)
{
	if (isatty(STDIN_FILENO) && shell_type == MAIN_SHELL)
		write(2, "exit\n", 5);
	if (args && args[1] != NULL)
		ft_check_arg(shell, args[1]);
	if (args && args[1] != NULL && args[2] != NULL)
		return (ft_too_many_args(shell, "exit", 1));
	if (fd > 0)
		close(fd);
	free_ast(shell->ast);
	ft_free_exit(shell);
}

void	ft_exit_ctrl_d(t_shell *shell, char **args, int shell_type, pid_t fd)
{
	if (isatty(STDIN_FILENO) && shell_type == MAIN_SHELL)
		write(2, "exit\n", 5);
	if (args && args[1] != NULL)
		ft_check_arg(shell, args[1]);
	if (args && args[1] != NULL && args[2] != NULL)
		return (ft_too_many_args(shell, "exit", 1));
	if (fd > 0)
		close(fd);
	ft_free_exit(shell);
}
