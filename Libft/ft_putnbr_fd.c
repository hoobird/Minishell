/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoobird <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 20:35:22 by hoobird           #+#    #+#             */
/*   Updated: 2023/09/19 00:51:06 by hoobird          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nn;

	nn = (long) n;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		nn = -nn;
	}
	if (nn >= 10)
		ft_putnbr_fd(nn / 10, fd);
	ft_putchar_fd('0' + nn % 10, fd);
}
