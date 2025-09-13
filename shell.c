/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:31:12 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/13 16:37:36 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clone_exp(t_shell *shell)
{
	t_strvec	*new;
	size_t		i;

	new = ft_strvec_init(shell->env->cap);
	if (new == NULL)
		ft_critical_error(shell);
	i = 0;
	while (i < shell->env->len)
	{
		if (shell->env->data[i][0] == '_' && shell->env->data[i][1] == '=')
		{
			i++;
			continue ;
		}
		new = ft_strvec_push(&new, fts_strdup(shell, shell->env->data[i]));
		if (new == NULL)
			ft_critical_error(shell);
		i++;
	}
	shell->exp = new;
}

int	ft_rl_eventloop(void)
{
	return (0);
}

void	ft_init_shell(t_shell *shell, char *envp[])
{
	ft_clone_env(shell, envp);
	ft_clone_exp(shell);
	ft_shlvl(shell);
	ft_set_pwd(shell);
	ft_set_default_path(shell);
	rl_event_hook = ft_rl_eventloop;
}

void	ft_critical_error(t_shell *shell)
{
	rl_clear_history();
	if (shell->input)
		free(shell->input);
	ft_strvec_free(shell->env);
	ft_strvec_free(shell->exp);
	ft_free_lexer(shell->lexer);
	if (shell->pwd)
		free(shell->pwd);
	if (shell->prompt)
		free(shell->prompt);
	if (shell->ast)
		free_ast(shell->ast);
	exit(EXIT_FAILURE);
}

void	ft_critical_with_code(t_shell *shell, int code, \
		t_pipe_parameters *tpp, pid_t *pids)
{
	// if (commands != NULL)
	// 	free_commands(commands);
	if (tpp != NULL)
		free_tpp(tpp, tpp->count - 1);
	if (pids != NULL)
		free(pids);
	shell->exit_status = code;
	rl_clear_history();
	if (shell->input)
		free(shell->input);
	ft_strvec_free(shell->env);
	ft_strvec_free(shell->exp);
	ft_free_lexer(shell->lexer);
	if (shell->pwd)
		free(shell->pwd);
	if (shell->prompt)
		free(shell->prompt);
	if (shell->ast)
		free_ast(shell->ast);
	exit(shell->exit_status);
}
