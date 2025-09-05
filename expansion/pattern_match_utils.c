/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_match_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:23:52 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:25:35 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
