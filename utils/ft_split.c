/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 12:01:14 by mel-badd          #+#    #+#             */
/*   Updated: 2025/09/13 13:45:42 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

static int	ft_countwords(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] == c && s[i])
		i++;
	while (s[i] != c && s[i])
	{
		while (s[i] != c && s[i])
			i++;
		count++;
		while (s[i] == c && s[i])
			i++;
	}
	return (count);
}

static char	*ft_put_words(const char *s, char c)
{
	char	*str;
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (s[len] && s[len] != c)
		len++;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static void	ft_free(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

static char	**ft_fill_array(const char *s, char **str, int len, char c)
{
	int	i;

	i = 0;
	while (i < len)
	{
		while (*s == c)
			s++;
		if (*s)
			str[i] = ft_put_words(s, c);
		if (!str[i])
		{
			ft_free(str);
			return (0);
		}
		while (*s && *s != c)
			s++;
		i++;
	}
	str[i] = 0;
	return (str);
}

char	**ft_split(char  *s, char c)
{
	int		len;
	char	**str;

	if (!s)
		return (NULL);
	len = ft_countwords(s, c);
	str = malloc(sizeof(char *) * (len + 1));
	if (!str)
		return (NULL);
	str = ft_fill_array(s, str, len, c);
	return (str);
}
