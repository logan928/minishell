/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:31:56 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/09 15:00:37 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2, char const *s3)
{
	char	*new;
	size_t	s1_len;
	size_t	s2_len;
	size_t	s3_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s3 && *s3)
		s3_len = ft_strlen(s3);
	else
		s3_len = 0;
	new = (char *)malloc(sizeof(char) * (s1_len + s2_len + s3_len + 1));
	if (!new)
		return (NULL);
	ft_memcpy(new, s1, s1_len);
	ft_memcpy(new + s1_len, s2, s2_len);
	if (s3 && *s3)
		ft_memcpy(new + s1_len + s2_len, s3, s3_len);
	new[s1_len + s2_len + s3_len] = '\0';
	return (new);
}
