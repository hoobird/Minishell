/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:51:42 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 15:51:50 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check file type directory or file
// return 1 if regular file
// return 2 if directory
// return 0 if other type of file
int	check_file_type(char *path)
{
	struct stat	filestat;

	if (stat(path, &filestat) == -1)
		return (-1);
	if ((filestat.st_mode & S_IFMT) == S_IFREG)
		return (1);
	else if ((filestat.st_mode & S_IFMT) == S_IFDIR)
		return (2);
	else
		return (0);
}

// check for read / write permissions
// possible mode are R_OK, W_OK, X_OK, F_OK
// R_OK - check for read permission
// W_OK - check for write permission
// X_OK - check for execute permission (DONT NEED THIS FOR NOW)
// F_OK - check for file existence
// return 0 if permission denied
// return 1 if permission granted
int	check_file_permissions(char *filename, int mode)
{
	if (access(filename, mode) == -1)
	{
		return (0);
	}
	return (1);
}
