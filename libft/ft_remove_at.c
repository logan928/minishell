#include "libft.h"

void	ft_remove_at(char **arr, size_t idx)
{
	size_t	size;

	size = 0;
	while (arr[size] != NULL)
		size++;
	if (idx >= size)
		return ;
	while (idx < size)
	{
		arr[idx] = arr[idx + 1];
		idx++;
	}
}
