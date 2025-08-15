/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 18:00:07 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/15 18:05:41 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_match(const char *pattern, const char *filename)
{
	DIR *dir;

	dir = opendir(".");
	closedir(dir);
	return (1);
}
