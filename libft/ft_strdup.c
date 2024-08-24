/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoobird <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 01:53:01 by hoobird           #+#    #+#             */
/*   Updated: 2023/09/19 02:14:37 by hoobird          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		count;
	char	*temp;

	count = 0;
	temp = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (temp == NULL)
		return (NULL);
	while ((size_t) count < ft_strlen(s))
	{
		temp[count] = s[count];
		count++;
	}
	return (temp);
}
