/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 15:58:52 by hulim             #+#    #+#             */
/*   Updated: 2023/09/17 16:39:40 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*dest1;
	char	*src1;

	if (dest == NULL && src == NULL)
		return (dest);
	dest1 = (char *) dest;
	if (dest >= src)
	{
		src1 = (char *) src;
		while (n > 0)
		{
			dest1[n - 1] = src1[n - 1];
			n--;
		}
	}
	else
	{
		ft_memcpy(dest1, src, n);
	}
	return (dest1);
}
