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

int	ft_set_quote_advance(const char *pattern, int *p, int *quote)
{
	if (ft_isquote(pattern[*p]) && !*quote)
	{
		*quote = pattern[*p];
		(*p)++;
		return (1);
	}
	else if (ft_isquote(pattern[*p]) && *quote == pattern[*p])
	{
		*quote = 0;
		(*p)++;
		return (1);
	}
	return (0);
}

int	ft_match(const char *pattern, const char *filename, int p, int f)
{
	int	star;
	int	match;
	int	quote;

	star = -1;
	match = 0;
	quote = 0;
	if (filename[0] == '.' && pattern[0] != '.')
		return (0);
	if (pattern[0] == '.' && pattern[1] == '/')
		p += 2;
	while (filename[f])
	{
		if (ft_set_quote_advance(pattern, &p, &quote))
			continue ;
		/*if (ft_isquote(pattern[p]) && !quote)
		{
			quote = pattern[p];
			p++;
			continue ;
		}
		else if (ft_isquote(pattern[p]) && quote == pattern[p])
		{
			quote = 0;
			p++;
			continue ;
		}*/
		if (pattern[p] == filename[f])
		{
			p++;
			f++;
		}
		else if (pattern[p] == '*')
		{
			star = p++;
			match = f;
		}
		else if (star != -1)
		{
			p = star + 1;
			f = ++match;
		}
		else
			return (0);
	}
	while (pattern[p] == '*')
		p++;

	return (pattern[p] == '\0');
}

//TODO: fix ./.* and ./*
t_token	*ft_matcher(const char *pattern)
{
	DIR				*dir;
	struct dirent	*dr;
	t_token			*new_token;
	t_token			*matches;

	matches = NULL;
	dir = opendir(".");
	while (1)
	{
		dr = readdir(dir);
		if (!dr)
			break ;
		if (ft_match(pattern, dr->d_name, 0, 0))
		{
			if (pattern[0] == '.' && pattern[1] == '/')
				new_token = ft_new_token(WORD, ft_strjoin_free(ft_strdup("./"), ft_strdup(dr->d_name)));
			else
				new_token = ft_new_token(WORD, ft_strdup(dr->d_name));
			ft_add_token(&matches, new_token);
		}
	}
	closedir(dir);
	return (matches);
}

int	ft_is_pattern(char *word)
{
	int	pos;
	int	quote;

	quote = 0;
	pos = 0;
	while (word[pos])
	{
		if (ft_isquote(word[pos]) && !quote)
			quote = word[pos];
		else if (ft_isquote(word[pos]) && word[pos] == quote)
			quote = 0;
		else if (word[pos] == '*' && !quote)
			return (1);
		pos++;
	}
	return (0);
}

void	ft_filename_expansion(t_shell *shell)
{
	t_token	*t;
	t_token *matches;
	t_token	*tmp;

	t = shell->lexer->tokens;
	while (t)
	{
		if (t->token_kind == WORD && ft_is_pattern(t->data))
		{
			matches = ft_matcher(t->data);
			if (matches && matches->next)
			{
				free(t->data);
				t->data = ft_strdup(matches->data);
				tmp = matches;
				matches = matches->next;
				free(tmp->data);
				free(tmp);
				tmp = matches;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = t->next;
				t->next = matches;
			}
			else if (matches)
			{
				free(t->data);
				t->data = ft_strdup(matches->data);
				free(matches->data);
				free(matches);
			}
		}
		t = t->next;
	}
}
