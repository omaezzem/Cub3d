/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:59:59 by omaezzem          #+#    #+#             */
/*   Updated: 2025/09/13 14:29:15 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

int	len_h(char **line)
{
	int	i;

	i = 0;
	while (line[i])
		i++;
	return (i);
}

size_t ft_strlenn(char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return i;
}


void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write (fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		ft_putchar_fd(s[i], fd);
		i++;
	}
}
