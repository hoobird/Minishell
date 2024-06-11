/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 17:12:05 by hulim             #+#    #+#             */
/*   Updated: 2023/09/10 16:59:53 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	char	*temp;
	int		count;

	temp = (char *) s;
	count = 0;
	while (*temp)
	{
		count++;
		temp++;
	}
	return (count);
}
