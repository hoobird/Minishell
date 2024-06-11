/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 15:11:25 by hulim             #+#    #+#             */
/*   Updated: 2023/09/17 17:25:45 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*subsub(void)
{
	char	*sub;

	sub = malloc(sizeof(char));
	if (sub == NULL)
		return (NULL);
	sub[0] = 0;
	return (sub);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	int		i;

	if (s == NULL)
		return (NULL);
	if (ft_strlen(s) < start)
		return (subsub());
	else
	{
		if (start + len > ft_strlen(s))
			len = ft_strlen(s) - start;
		sub = malloc(sizeof(char) * (len + 1));
		if (sub == NULL)
			return (NULL);
		i = -1;
		while (++i < (int) len)
			sub[i] = s[start + i];
		sub[i] = 0;
		return (sub);
	}
}
