/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:05:13 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/27 16:27:01 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	//if (argc > 1)
	//	return(EXIT_SUCCESS);
	//if (!isatty(STDIN_FILENO))
//		return(EXIT_SUCCESS);
	(void)argc;
	(void)argv;
	shell = (t_shell){NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, 0};
	ft_init_shell(&shell, envp);
	shell.lexer = &(t_lexer){NULL, 0, NULL, NULL, NULL};
	while (1)
	{
		shell.input = readline(ft_set_prompt(&shell));
		if (!shell.input)
			ft_exit(&shell, NULL, MAIN_SHELL);
		if (shell.input)
		{
			if (*shell.input)
				add_history(shell.input);
			else
			{
				free(shell.input);
				continue ;
			}
		}
		lex(&shell, shell.input);
		if (ft_check_syntax(&shell))
		{
			shell.parse_err = 0;
			ft_here(&shell);
			t_ast *root = parse(&shell, &shell.lexer->tokens);
			//print_ast(&shell, root, 0);//remove
			if (shell.parse_err != 0)
			{
				printf("Parse error: %d\n", shell.parse_err);
				shell.parse_err = 0;
			}
			else
				shell.exit_status = (unsigned char) exec_ast(&shell, root);
		}	
		//free_ast(root);// Check
		free(shell.input);
		ft_free_lexer(shell.lexer);
		if (g_sig)
			g_sig = 0;
	}
	return (0);
}
