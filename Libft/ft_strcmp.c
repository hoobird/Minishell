/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 02:36:41 by hulim             #+#    #+#             */
/*   Updated: 2024/07/23 02:37:34 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			count;
	unsigned char	*ss1;
	unsigned char	*ss2;

	count = 0;
	ss1 = (unsigned char *)s1;
	ss2 = (unsigned char *)s2;
	while (ss1[count] || ss2[count])
	{
		if (ss1[count] != ss2[count])
			return (ss1[count] - ss2[count]);
		count++;
	}
	return (0);
}
