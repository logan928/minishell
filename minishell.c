/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:05:13 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/06 14:24:36 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

char	*get_prompt(void)
{
	char	*prompt;
	char	*username;

	username = getenv("USER");
	prompt = malloc(sizeof(char) * (strlen(username) + 3));
	if (!prompt)
		return (NULL);
	memcpy(prompt, username, strlen(username));
	prompt[strlen(username)] = '$';
	prompt[strlen(username) + 1] = ' ';
	prompt[strlen(username) + 2] = '\0';
	return (prompt);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*line_read;
	char	**env;
	t_lexer	lexer;
	char	*prompt;

	(void)argc;
	(void)argv;
	env = ft_clone_env(envp);
	if (!env)
		return (0);
	rl_catch_signals = 0;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_trap);
	while (1)
	{
		prompt = get_prompt();
		line_read = readline(get_prompt());
		if (!line_read)
		{
			if (isatty(STDIN_FILENO))
				write(2, "exit\n", 6);
			break ;
		}
		if (line_read && *line_read)
			add_history(line_read);
		lexer = (t_lexer){line_read, NULL, 0, 0, 0, NULL};
		lex(&lexer);
		for (t_token *token = lexer.tokens; token; token = token->next)
			printf("Kind: %d, data: [%s]\n", token->token_kind, token->data);
		/*
		if (strcmp(line_read, "env") == 0)
			ft_env(env);
		else if (strcmp(line_read, "exit") == 0)
		{
			for (int i = 0; env[i] != NULL; i++)
				free(env[i]);
			free(env);
			free(line_read);
			exit(EXIT_SUCCESS);
		}
		*/
		free(line_read);
		if (g_sig)
			g_sig = 0;
	}
	rl_clear_history();
	return (0);
}
