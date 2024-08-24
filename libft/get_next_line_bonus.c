/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 20:09:17 by hulim             #+#    #+#             */
/*   Updated: 2024/04/25 20:11:04 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	*memline[4096];
	char		*nextline;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 4096)
		return (NULL);
	updatememline(fd, &(memline[fd]));
	if (gnl_strlen(memline[fd]) == 0 || !memline[fd])
	{
		if (memline[fd])
			free(memline[fd]);
		return (NULL);
	}
	nextline = splitline(memline[fd]);
	memline[fd] = transfermemline(&(memline[fd]));
	return (nextline);
}

char	*transfermemline(char **meml)
{
	int		len;
	char	*temp;
	int		i;

	len = checkfornewline(*meml);
	if (len == -1 || !(*meml)[len + 1])
	{
		free(*meml);
		return (NULL);
	}
	temp = (char *)gnl_calloc(gnl_strlen(*meml) - len, sizeof(char));
	if (!temp)
		return (NULL);
	len++;
	i = 0;
	while ((*meml)[len])
	{
		temp[i++] = (*meml)[len];
		len++;
	}
	free(*meml);
	return (temp);
}

char	*splitline(char *line)
{
	char	*output;
	int		len;
	int		count;

	len = checkfornewline(line);
	if (len == -1)
		output = gnl_calloc(gnl_strlen(line) + 1, sizeof(char));
	else
		output = gnl_calloc(len + 2, sizeof(char));
	if (!output)
		return (NULL);
	count = 0;
	while (line[count] && line[count] != '\n')
	{
		output[count] = line[count];
		count++;
	}
	if (len >= 0)
		output[count] = '\n';
	return (output);
}
