/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canonical_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:41:38 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/26 21:41:38 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_split_path(t_shell *shell, t_stack **st, char ***seg, char *cur)
{
	*st = ft_stack_init();
	if (*st == NULL)
	{
		free(cur);
		ft_critical_error(shell);
	}
	*seg = ft_split(cur, '/');
	if (!*seg)
	{
		free(cur);
		ft_stack_free(*st);
		ft_critical_error(shell);
	}
}

static bool	ft_check_dots(char *segment, t_stack **st)
{
	if (ft_strcmp(segment, ".", 0) == 0)
		return (true);
	if (ft_strcmp(segment, "..", 0) == 0)
	{
		if (ft_stack_peek(*st) != NULL)
			ft_stack_pop(*st);
		return (true);
	}
	return (false);
}

static void	ft_push_err(t_shell *sh, char *cur, char **seg, t_stack *st)
{
	free(cur);
	ft_stack_free(st);
	ft_free_arr(seg);
	ft_critical_error(sh);
}

char	*ft_canonicalize(t_shell *shell, char *curpath)
{
	t_stack	*st;
	char	**segments;
	size_t	i;
	char	*res;

	ft_split_path(shell, &st, &segments, curpath);
	i = 0;
	while (segments[i] != NULL)
	{
		if (ft_check_dots(segments[i], &st))
		{
			i++;
			continue ;
		}
		else if (ft_stack_push(st, segments[i]) == NULL)
			ft_push_err(shell, curpath, segments, st);
		i++;
	}
	res = ft_reconstruct_path(st);
	free(curpath);
	ft_stack_free(st);
	ft_free_arr(segments);
	return (res);
}
