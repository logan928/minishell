/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:05:13 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 17:40:56 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <string.h>

volatile sig_atomic_t	g_sig = 0;

char	*get_prompt(void)
{
	char	*prompt;
	char	*username;
	size_t	username_len;

	username = getenv("USER");
	if (!username)
		username = "minishell";
	username_len = ft_strlen(username);
	prompt = malloc(sizeof(char) * (username_len + 3));
	if (!prompt)
		return (NULL);
	ft_memcpy(prompt, username, username_len);
	prompt[username_len] = '$';
	prompt[username_len + 1] = ' ';
	prompt[username_len + 2] = '\0';
	return (prompt);
}

void	ft_free_exit(t_shell *shell)
{
	ft_free_lexer(shell->lexer);
	free(shell->prompt);
	ft_free_env(shell->env);
	rl_clear_history();
	if (isatty(STDIN_FILENO))
		write(2, "exit\n", 6);
	exit(shell->exit_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;
	//t_token	*token;
	//int		syntax_status;

	(void)argc;
	(void)argv;
	shell = (t_shell){NULL, 0, NULL, NULL, NULL};
	ft_init_shell(&shell, envp);
	shell.lexer = &(t_lexer){NULL, 0, 0, 0, NULL, NULL};
	while (1)
	{
		shell.input = readline(shell.prompt);
		if (!shell.input)
			ft_free_exit(&shell);
		if (shell.input)
		{
			if (*shell.input)
				add_history(shell.input);
			else
			{	
				free(shell.input);
				continue;
			}
		}
		lex(&shell, shell.input, shell.lexer);
		//syntax_status = ft_check_syntax(shell.lexer);
		//ft_variable_expansion(&shell);
		//ft_field_splitting(&shell);
		//ft_filename_expansion(&shell);
		//ft_quote_removal(&shell);
	//if (syntax_status == 1)
	//{
	//	token = shell.lexer->tokens;
	//		while (token)
	//		{
	//			printf("Kind: %d, data: [%s]\n", token->token_kind, token->data);
	//		token = token->next;
	//		}
//		}
		//t_command *cmd = command_formatter(&shell.lexer->tokens);
		//print_lexem(cmd);
		t_ast *root = parse(&shell.lexer->tokens);
		print_ast(&shell, root, 0);//remove 
		//free_ast(root);// Check

		free(shell.input);
		ft_reset_lexer(shell.lexer);
		if (g_sig)
			g_sig = 0;
	}
	return (0);
}
