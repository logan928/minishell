/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:05:13 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/10 13:45:22 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_abort;

void	ft_run_lex(t_shell *shell)
{
	t_token	*tok;
	
	lex(shell, shell->input);
	tok = shell->lexer->tokens;
	if (ft_check_syntax(shell))
	{
		shell->parse_err = 0;
		ft_here(shell);
		if (g_abort != HEREDOC_INT)
		{
			shell->ast = parse(shell, &tok);
			if (shell->parse_err != 0)
				shell->parse_err = 0;
			else
				shell->exit_status = exec_ast(shell, shell->ast);
			free_ast(shell->ast);
		}
	}
	free(shell->input);
	ft_free_lexer(shell->lexer);
}

void	ft_interactive(t_shell *shell)
{
	while (1)
	{
		if (g_abort)
		{
			shell->exit_status = g_abort;
			g_abort = 0;
		}
		ft_set_signals_main_pre();
		shell->input = readline(ft_set_prompt(shell));
		ft_set_signals_post();
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
	free(input);
	ft_free_exit(shell);
}

void	ft_non_interactive_piped(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = ft_get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		shell->input = line;
		ft_run_lex(shell);
	}
	ft_free_exit(shell);
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
	else if (isatty(STDIN_FILENO))
		ft_interactive(&shell);
	else
		ft_non_interactive_piped(&shell);
}
