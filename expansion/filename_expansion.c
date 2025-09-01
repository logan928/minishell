/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:39:12 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/01 16:37:08 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_add_match_token(t_shell *shell, int prefix_len,
							const char *pattern, char *filename)
{
	char	*prefix;
	char	*base;
	t_token	*new;

	if (prefix_len > 0)
	{
		prefix = ft_strndup_safe(shell, pattern, prefix_len);
		base = ft_strdup_safe(shell, filename);
		new = ft_new_token_safe(shell, WORD,
				ft_strjoin_free_safe(shell, prefix, base));
	}
	else
	{
		base = ft_strdup_safe(shell, filename);
		new = ft_new_token_safe(shell, WORD, base);
	}
	ft_add_token_sorted(&shell->lexer->tmp, new);
}

static void	ft_glob_dir(t_shell *shell, const char *pattern)
{
	DIR				*dir;
	struct dirent	*dr;
	int				prefix_len;

	prefix_len = 0;
	while (pattern[prefix_len] == '.' && pattern[prefix_len + 1] == '/')
		prefix_len += 2;
	dir = opendir(".");
	if (!dir)
		ft_critical_error(shell);
	while (dir)
	{
		dr = readdir(dir);
		if (!dr)
			break ;
		if (ft_pattern_match(pattern + prefix_len, dr->d_name))
			ft_add_match_token(shell, prefix_len, pattern, dr->d_name);
	}
	closedir(dir);
}

void	ft_copy_single_item(t_shell *sh, char *s)
{
	char	*copy;
	t_token	*t;

	copy = ft_strdup_safe(sh, s);
	t = ft_new_token_safe(sh, WORD, copy);
	ft_add_token(&sh->lexer->tmp, t);
}

void	ft_filename_expansion(t_shell *sh, char ***arr, size_t idx, int is_cmd)
{
	size_t	lst_size;

	sh->lexer->tmp2 = NULL;
	while ((*arr)[idx])
	{
		sh->lexer->tmp = NULL;
		if (ft_is_pattern((*arr)[idx]))
		{
			ft_glob_dir(sh, (*arr)[idx]);
			if (sh->lexer->tmp == NULL)
				ft_copy_single_item(sh, (*arr)[idx]);
		}
		else
			ft_copy_single_item(sh, (*arr)[idx]);
		ft_add_token(&sh->lexer->tmp2, sh->lexer->tmp);
		idx++;
	}
	lst_size = ft_lst_size(sh->lexer->tmp2);
	sh->lexer->tmp = sh->lexer->tmp2;
	ft_merge(sh, arr, lst_size, is_cmd);
	sh->lexer->tmp2 = NULL;
}
