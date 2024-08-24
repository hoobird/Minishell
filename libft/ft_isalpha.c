/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 15:02:45 by hulim             #+#    #+#             */
/*   Updated: 2023/09/07 15:24:40 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
/*
#include <stdio.h>
int	main(void)
{
	char c;
	
	c = 'a';
	printf("%d", ft_isalpha(c));
	c = 'A';
	printf("%d", ft_isalpha(c));
	c = '4';
	printf("%d", ft_isalpha(c));
}
*/
