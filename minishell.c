/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:05:13 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 17:57:00 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t g_abort;

void	ft_run_lex(t_shell *shell)
{
	lex(shell, shell->input);
	ft_skip_empty_vars(shell);
	if (ft_check_syntax(shell))
	{
		shell->parse_err = 0;
		if (isatty(STDIN_FILENO))
			ft_here(shell);
		if (g_abort != HEREDOC_INT)
		{
			shell->ast = parse(shell, &shell->lexer->tokens);
			if (shell->parse_err != 0)
				shell->parse_err = 0;
			else
				shell->exit_status = (unsigned char) exec_ast(shell, shell->ast);
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

void	ft_non_interactive(t_shell *shell)
{
	char	*line;
	char	*input;
	char	*newline;

	line = ft_get_next_line(STDIN_FILENO);
	if (!line || line[0] == '\0' || line[0] == '\n')
		return ;
	newline = ft_strchr(line, '\n');
	if (newline)
	{
		input = ft_strndup_safe(shell, line, ft_strlen(line) - 1);
		free(line);
		shell->input = input;
	}
	else
		shell->input = line;
	ft_run_lex(shell);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell = (t_shell){NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, 0};
	ft_init_shell(&shell, envp);
	shell.lexer = &(t_lexer){NULL, 0, NULL, NULL, NULL};
	ft_interactive(&shell);
	return (0);
}
