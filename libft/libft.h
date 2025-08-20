/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:06:12 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/18 10:43:40 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>

int		ft_isquote(char c);
int		ft_isspace(char c);
int		ft_isdigit(int c);
char	*ft_itoa(long n);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strjoin(char const *s1, char const *s2, char const *s3);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin_free(char *s1, char *s2);
int		ft_strcmp(const char *s1, const char *s2, int ignore_case);

#endif
