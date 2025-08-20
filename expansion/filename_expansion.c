/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:39:12 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/20 18:07:51 by mkugan           ###   ########.fr       */
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
	shell->lexer->tmp = NULL;
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

static void	ft_replace_with_matches(t_shell *shell, t_token *t)
{
	t_token	*tmp_token;

	if (shell->lexer->tmp && shell->lexer->tmp->next)
	{
		free(t->data);
		t->data = ft_strdup_safe(shell, shell->lexer->tmp->data);
		tmp_token = shell->lexer->tmp;
		shell->lexer->tmp = shell->lexer->tmp->next;
		free(tmp_token->data);
		free(tmp_token);
		tmp_token = shell->lexer->tmp;
		while (tmp_token->next)
			tmp_token = tmp_token->next;
		tmp_token->next = t->next;
		t->next = shell->lexer->tmp;
		shell->lexer->tmp = NULL;
	}
	else if (shell->lexer->tmp)
	{
		free(t->data);
		t->data = ft_strdup_safe(shell, shell->lexer->tmp->data);
		ft_free_tokens(shell->lexer->tmp);
		shell->lexer->tmp = NULL;
	}
}

void	ft_filename_expansion(t_shell *shell)
{
	t_token	*t;

	t = shell->lexer->tokens;
	while (t)
	{
		if (t->token_kind == WORD && ft_is_pattern(t->data))
		{
			ft_glob_dir(shell, t->data);
			ft_replace_with_matches(shell, t);
		}
		t = t->next;
	}
}
