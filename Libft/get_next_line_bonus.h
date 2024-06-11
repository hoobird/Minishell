/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 20:09:20 by hulim             #+#    #+#             */
/*   Updated: 2024/04/25 21:21:03 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
void	updatememline(int fd, char **memline);
void	combinelines(char **memline, char **buf);
char	*transfermemline(char **meml);
int		checkfornewline(char *memline);
void	*gnl_calloc(size_t count, size_t size);
int		gnl_strlen(char *str);
char	*splitline(char *line);

#endif
