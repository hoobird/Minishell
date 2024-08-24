/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 15:58:52 by hulim             #+#    #+#             */
/*   Updated: 2023/09/17 15:30:21 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*dest1;
	char	*src1;
	size_t	i;

	if (dest == NULL && src == NULL)
		return (dest);
	dest1 = (char *) dest;
	src1 = (char *) src;
	i = 0;
	while (i < n)
	{
		*dest1 = *src1;
		dest1++;
		src1++;
		i++;
	}
	return (dest);
}
