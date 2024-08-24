/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 20:09:55 by hulim             #+#    #+#             */
/*   Updated: 2024/04/25 20:11:12 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	updatememline(int fd, char **memline)
{
	char	*buf;
	int		chars;

	if (checkfornewline(*memline) >= 0)
		return ;
	buf = (char *)gnl_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buf)
		return ;
	chars = read(fd, buf, BUFFER_SIZE);
	if (chars == -1)
	{
		free(*memline);
		*memline = NULL;
	}
	if (chars <= 0)
	{
		free(buf);
		return ;
	}
	combinelines(memline, &buf);
	updatememline(fd, memline);
	return ;
}

void	combinelines(char **memline, char **buf)
{
	char	*temp;
	int		c;
	int		d;

	temp = (char *)gnl_calloc(gnl_strlen(*memline) + gnl_strlen(*buf) + 1,
			sizeof(char));
	if (!temp)
		return ;
	c = 0;
	while (*memline && (*memline)[c])
	{
		temp[c] = (*memline)[c];
		c++;
	}
	d = 0;
	while (*buf && (*buf)[d])
	{
		temp[c + d] = (*buf)[d];
		d++;
	}
	free(*memline);
	free(*buf);
	*memline = temp;
}

int	checkfornewline(char *memline)
{
	int	c;

	if (!memline)
		return (-1);
	c = 0;
	while (memline[c])
	{
		if (memline[c] == '\n')
			return (c);
		c++;
	}
	return (-1);
}

void	*gnl_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	c;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	c = 0;
	while (c < count * size)
	{
		((char *)ptr)[c] = 0;
		c++;
	}
	return (ptr);
}

int	gnl_strlen(char *str)
{
	int	c;

	if (!str)
		return (0);
	c = 0;
	while (str[c])
		c++;
	return (c);
}
