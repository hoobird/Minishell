/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   learnexit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:47:15 by hulim             #+#    #+#             */
/*   Updated: 2024/08/22 17:47:16 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	builtin_exit(99);
	write(1, "This should not be printed\n", 28);
	return (0);
}
