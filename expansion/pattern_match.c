/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_match.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:43:32 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/22 13:43:33 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_handle_quote(const char *pattern, int *p, int *quote)
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

static int	ft_step_match(const char *pattern, const char *filename,
					t_glob_state *g)
{
	if (pattern[g->pattern_index] == filename[g->filename_index])
	{
		g->pattern_index++;
		g->filename_index++;
	}
	else if (pattern[g->pattern_index] == '*')
	{
		g->last_star_index = g->pattern_index++;
		g->star_match_index = g->filename_index;
	}
	else if (g->last_star_index != -1)
	{
		g->pattern_index = g->last_star_index + 1;
		g->filename_index = ++g->star_match_index;
	}
	else
		return (0);
	return (1);
}

int	ft_pattern_match(const char *pattern, const char *filename)
{
	t_glob_state	g;

	g = (t_glob_state){-1, 0, 0, 0, 0};
	if (filename[0] == '.' && pattern[0] != '.')
		return (0);
	while (filename[g.filename_index])
	{
		if (ft_handle_quote(pattern, &g.pattern_index, &g.in_quotes))
			continue ;
		if (!ft_step_match(pattern, filename, &g))
			return (0);
	}
	while (pattern[g.pattern_index] == '*')
		g.pattern_index++;
	return (pattern[g.pattern_index] == '\0');
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
