/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:31:12 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 16:49:54 by mkugan           ###   ########.fr       */
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

void	ft_init_shell(t_shell *shell, char *envp[])
{
	ft_clone_env(shell, envp);
	if (!shell->env)
		exit(EXIT_FAILURE);
	ft_clone_exp(shell);
	ft_shlvl(shell);
	ft_set_pwd(shell);
	rl_catch_signals = 0;
}

void	ft_critical_error(t_shell *shell)
{
	rl_clear_history();
	if (shell->input)
		free(shell->input);
	if (shell->env)
		ft_strvec_free(shell->env);
	if (shell->exp)
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
