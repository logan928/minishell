/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:05:13 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/08 17:18:07 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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

int	main(int argc, char *argv[], char *envp[])
{
	char	*line_read;
	char	**env;
	t_lexer	lexer;
	char	*prompt;
	t_token	*token;

	(void)argc;
	(void)argv;
	env = ft_clone_env(envp);
	if (!env)
		return (0);
	rl_catch_signals = 0;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_trap);
	prompt = get_prompt();
	if (!prompt)
		return 0;
	ft_env(env);
	while (1)
	{
		line_read = readline(prompt);
		if (!line_read)
		{
			free(prompt);
			ft_free_env(env);
			rl_clear_history();
			if (isatty(STDIN_FILENO))
				write(2, "exit\n", 6);
			break ;
		}
		if (line_read && *line_read)
			add_history(line_read);
		lexer = (t_lexer){line_read, NULL, 0, 0, 0, NULL};
		lex(&lexer);
		ft_check_syntax(&lexer);
		if (lexer.tokens)
		{
			token = lexer.tokens;
			while (token)
			{
				printf("Kind: %d, data: [%s]\n", token->token_kind, token->data);
				token = token->next;
			}
		}
		free(prompt);
		free(line_read);
		if (g_sig)
			g_sig = 0;
	}
	rl_clear_history();
	return (0);
}
