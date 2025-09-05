/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fancy_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:33:13 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:08:17 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_alloc_propmt(t_shell *shell, size_t u_len, size_t p_len)
{
	char	*home;
	size_t	prompt_len;
	size_t	home_len;
	int		ret;

	ret = 0;
	home = getenv("HOME");
	if (home != NULL)
	{
		home_len = ft_strlen(home);
		if (!ft_strncmp(home, shell->pwd, home_len)
			&& (shell->pwd[home_len] == '\0' || shell->pwd[home_len] == '/'))
		{
			ret = home_len;
			prompt_len = p_len - home_len + 1;
		}
		else
			prompt_len = p_len;
	}
	else
		prompt_len = p_len;
	prompt_len += u_len;
	prompt_len += 36;
	shell->prompt = fts_malloc(shell, prompt_len);
	return (ret);
}

static void	ft_build_prompt(t_shell *sh, char *u, size_t u_len, size_t p_len)
{
	size_t	offset;
	int		h_len;

	h_len = ft_alloc_propmt(sh, u_len, p_len);
	offset = 0;
	ft_memcpy(sh->prompt + offset, "\e[1;92m", 7);
	offset += 7;
	ft_memcpy(sh->prompt + offset, u, u_len);
	offset += u_len;
	ft_memcpy(sh->prompt + offset, "@minishell:", 11);
	offset += 11;
	ft_memcpy(sh->prompt + offset, "\e[0m", 4);
	offset += 4;
	ft_memcpy(sh->prompt + offset, "\e[1;94m", 7);
	offset += 7;
	if (h_len)
		ft_memcpy(sh->prompt + offset++, "~", 1);
	ft_memcpy(sh->prompt + offset, sh->pwd + h_len, p_len - h_len);
	offset += p_len - h_len;
	ft_memcpy(sh->prompt + offset, "\e[0m", 4);
	offset += 4;
	ft_memcpy(sh->prompt + offset, "$ ", 2);
	offset += 2;
	sh->prompt[offset] = '\0';
}

char	*ft_set_prompt(t_shell *shell)
{
	char	*username;
	size_t	username_len;
	size_t	pwd_len;

	if (shell->prompt != NULL)
	{
		free(shell->prompt);
		shell->prompt = NULL;
	}
	username = getenv("USER");
	if (!username)
		username = "anonymous";
	username_len = ft_strlen(username);
	pwd_len = ft_strlen(shell->pwd);
	ft_build_prompt(shell, username, username_len, pwd_len);
	return (shell->prompt);
}
