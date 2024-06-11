/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoobird <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 02:15:46 by hoobird           #+#    #+#             */
/*   Updated: 2023/09/19 02:25:45 by hoobird          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*output;
	int		counter;

	output = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!s || output == NULL)
		return (NULL);
	counter = 0;
	while (s[counter] != 0)
	{
		output[counter] = f(counter, s[counter]);
		counter++;
	}
	output[counter] = 0;
	return (output);
}
