/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:59:48 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/25 15:25:01 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_exit(t_shell *shell)
{
	ft_free_lexer(shell->lexer);
	free(shell->prompt);
	ft_free_arr(shell->env);
	free(shell->pwd);
	rl_clear_history();
	exit(shell->exit_status);
}

void	ft_num_arg_req(t_shell *shell, char *cmd, char *arg)
{
	char	*err;

	shell->exit_status = (unsigned char) 2;
	err = ft_str_join3_cpy_safe(shell, cmd, arg, ENUMREQ);
	ft_write_safe(shell, err, STDERR_FILENO);
	ft_free_exit(shell);
}

static void	ft_check_arg(t_shell *shell, char *arg)
{
	char		*end;
	long long	n;

	if (!ft_is_valid_number(arg))
		return (ft_num_arg_req(shell, "minishell: exit: ", arg));
	n = ft_strtoll(arg, &end, 10);
	if (*end != '\0')
		return (ft_num_arg_req(shell, "minishell: exit: ", arg));
	shell->exit_status = (unsigned char) n;
}

void	ft_exit(t_shell *shell, char **args)
{
	if (isatty(STDIN_FILENO))
		write(2, "exit\n", 6);
	if (args && args[1] != NULL)
		ft_check_arg(shell, args[1]);
	if (args && args[1] != NULL && args[2] != NULL)
		return (ft_too_many_args(shell, "exit", 2));
	ft_free_exit(shell);
}
