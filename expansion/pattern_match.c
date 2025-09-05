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

static int	ft_match(const char *pattern, const char *filename, int in_quotes);

int	ft_first_unquoted_char(const char *pattern)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (pattern[i])
	{
		if (ft_isquote(pattern[i]) && !quote)
			quote = pattern[i];
		else if (ft_isquote(pattern[i]) && quote)
			quote = 0;
		else
			return (pattern[i]);
		i++;
	}
	return (0);
}

static int	ft_quote(const char **p, int *in_q)
{
	if (!*in_q)
	{
		*in_q = **p;
		(*p)++;
		return (1);
	}
	else if (*in_q == **p)
	{
		*in_q = 0;
		(*p)++;
		return (1);
	}
	return (0);
}

static int	ft_star_in_qoute(const char **p, const char **f)
{
	if (**f != '*')
		return (0);
	(*p)++;
	(*f)++;
	return (1);
}

static int ft_star_meta(const char **p, const char **f, int *in_q)
{
	(*p)++;
	while (**f)
	{
		if (ft_match(*p, *f, *in_q))
			return (1);
		(*f)++;
	}
	return (ft_match(*p, *f, *in_q));
}

static int	ft_match(const char *pattern, const char *filename, int in_quotes)
{
	while (*pattern)
	{
		if (ft_isquote(*pattern))
		{
			if (!ft_quote(&pattern, &in_quotes))
				return (0);
			continue ;
		}
		if (*pattern == '*' && in_quotes)
		{
			if (!ft_star_in_qoute(&pattern, &filename))
				return (0);
			continue;
		}
		if (*pattern == '*')
			return (ft_star_meta(&pattern, &filename, &in_quotes));
		if (*pattern != *filename)
			return (0);
		pattern++;
		filename++;
	}
	return (*filename == '\0');
}

int	ft_pattern_match(const char *pattern, const char *filename)
{
	if (!ft_strcmp(filename, ".", 0) || !ft_strcmp(filename, "..", 0))
		return 0;
	if (filename[0] == '.' && ft_first_unquoted_char(pattern) != '.')
		return 0;
	return ft_match(pattern, filename, 0);
}
