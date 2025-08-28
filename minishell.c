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
	int		syntax_status;

	(void)argc;
	(void)argv;
	shell = (t_shell){NULL, 0, NULL, NULL, NULL, NULL, NULL};
	ft_init_shell(&shell, envp);
	shell.lexer = &(t_lexer){NULL, 0, 0, 0, 0, NULL, NULL};
	while (1)
	{
		shell.input = readline(ft_set_prompt(&shell));
		if (!shell.input)
			ft_exit(&shell, NULL);
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
		lex(&shell, shell.input, shell.lexer);
		syntax_status = ft_check_syntax(&shell);
			if (syntax_status && shell.lexer->tokens->token_kind != NL)
		{
			ft_here(&shell);
			t_ast *root = parse(&shell);
			//print_ast(&shell, root, 0);//remove 
			exec_ast(&shell, root);

		}	
		//free_ast(root);// Check
		free(shell.input);
		ft_reset_lexer(shell.lexer);
		if (g_sig)
			g_sig = 0;
	}
	return (0);
}
