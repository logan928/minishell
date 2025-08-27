/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:06:12 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/26 15:10:43 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <limits.h>

# define BUFFER_SIZE 1024

typedef struct s_char_node	t_char_node;

typedef struct	s_char_node
{
	char			*value;
	t_char_node		*prev;
}	t_char_node;

typedef struct	s_stack
{
	t_char_node	*head;
	size_t		size;
}	t_stack;

int			ft_isquote(char c);
int			ft_isspace(char c);
int			ft_isdigit(int c);
char		*ft_itoa(long n);
char		*ft_strndup(const char *s, size_t n);
char		*ft_strdup(const char *s);
size_t		ft_strlen(const char *s);
void		*ft_memcpy(void *dest, const void *src, size_t n);
char		*ft_strjoin(char const *s1, char const *s2, char const *s3);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strjoin_free(char *s1, char *s2);
int			ft_strcmp(const char *s1, const char *s2, int ignore_case);
void		*ft_calloc(size_t nmemb, size_t size);
void		*ft_memset(void *s, int c, size_t n);
char		*ft_strchr(const char *s, int c);
char		**ft_split(char const *s, char c);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
void		ft_free_arr(char **arr);
void		*ft_memmove(void *dest, const void *src, size_t n);
char		*ft_get_next_line(int fd);
long long	ft_strtoll(const char *str, char **str_end, int base);
int			ft_tolower(int c);
int			ft_isupper(int c);
size_t		ft_strlcat(char *dst, const char *src, size_t size);

t_stack		*ft_stack_init(void);
t_stack		*ft_stack_push(t_stack *st, char *value);
char		*ft_stack_pop(t_stack *st);
void		ft_stack_free(t_stack *st);
char		*ft_stack_peek(t_stack *st);
char		**ft_realloc_arr(char **old);
void		ft_remove_at(char **arr, size_t idx);

#endif
