/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 15:25:19 by hulim             #+#    #+#             */
/*   Updated: 2023/09/07 15:46:21 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>
int	main(void)
{
	char x,y,z;
	x = '1';
	y = '9';
	z = 'a';
	printf("%d\n%d\n%d", isdigit(x), isdigit(y), isdigit(z));
	return 1;
}
*/
