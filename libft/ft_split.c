/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 15:09:31 by hulim             #+#    #+#             */
/*   Updated: 2023/09/19 20:35:46 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**freeall(char **arr)
{
	int	count;

	count = 0;
	while (arr[count])
	{
		free(arr[count]);
		count++;
	}
	free(arr[count]);
	free(arr);
	return (NULL);
}

size_t	countwords(char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s != c)
			{
				s++;
				if (*s == 0)
					return (count);
			}
		}
		s++;
	}
	return (count);
}

char	**testt(int *ii, char **ss, char const *s, char c)
{
	*ii = 0;
	*ss = (char *) s;
	return ((char **) malloc(sizeof(char *) * (countwords(*ss, c) + 1)));
}

void	testb(char **ff, char **ss, char **bb, char c)
{
	*ff = *ss;
	while (**ss != c && **ss)
		(*ss)++;
	*bb = *ss - 1;
}

char	**ft_split(char const *s, char c)
{
	char	*ss;
	char	**output;
	char	*f;
	char	*b;
	int		i;

	output = testt(&i, &ss, s, c);
	if (output == NULL)
		return (NULL);
	output[countwords(ss, c)] = NULL;
	while (*ss)
	{
		if (*ss != c)
		{
			testb(&f, &ss, &b, c);
			output[i] = malloc(sizeof(char) * (b - f + 2));
			if (output[i] == NULL)
				return (freeall(output));
			ft_strlcpy(output[i++], f, b - f + 2);
		}
		if (*ss++ == 0)
			break ;
	}
	return (output);
}
