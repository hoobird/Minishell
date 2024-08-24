/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 17:14:47 by hulim             #+#    #+#             */
/*   Updated: 2023/09/19 03:57:44 by hoobird          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	count;
	int	len;

	count = 0;
	len = ft_strlen(s);
	while (count <= len)
	{
		if (s[count] == (unsigned char) c)
		{
			return ((char *)&s[count]);
		}
		count++;
	}
	return (NULL);
}
