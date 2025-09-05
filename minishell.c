/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:05:13 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 16:50:07 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_abort;

void	ft_run_lex(t_shell *shell)
{
	lex(shell, shell->input);
	if (ft_check_syntax(shell))
	{
		shell->parse_err = 0;
		ft_here(shell);
		if (g_abort != HEREDOC_INT)
		{
			shell->ast = parse(shell, &shell->lexer->tokens);
			if (shell->parse_err != 0)
				shell->parse_err = 0;
			else
				shell->exit_status = exec_ast(shell, shell->ast);
		}
	}
	free(shell->input);
	if (g_abort == HEREDOC_INT)
		shell->exit_status = 130;
	ft_free_lexer(shell->lexer);
}

void	ft_interactive(t_shell *shell)
{
	while (1)
	{
		shell->input = readline(ft_set_prompt(shell));
		if (!shell->input)
			ft_exit(shell, NULL, MAIN_SHELL);
		if (shell->input)
		{
			if (*shell->input)
				add_history(shell->input);
			else
			{
				free(shell->input);
				continue ;
			}
		}
		g_abort = 0;
		shell->parse_err = 0;
		ft_run_lex(shell);
	}
}

void	ft_non_interactive(t_shell *shell, char **argv)
{
	char	**input;
	int		i;

	input = ft_split(argv[2], ';');
	if (!input)
		exit(0);
	i = 0;
	while (input[i])
	{
		shell->input = input[i];
		ft_run_lex(shell);
		i++;
	}
	exit(shell->exit_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;
	t_lexer	lexer;

	shell = (t_shell){NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, 0};
	ft_init_shell(&shell, envp);
	lexer = (t_lexer){NULL, 0, NULL, NULL, NULL};
	shell.lexer = &lexer;
	if (argc == 3 && ft_strcmp(argv[1], "-c", 0) == 0 && argv[2])
		ft_non_interactive(&shell, argv);
	else
	{
		signal(SIGINT, ft_sigint_handler);
		signal(SIGQUIT, ft_sigquit_trap);
		ft_interactive(&shell);
	}
}
