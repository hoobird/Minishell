/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:52:18 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 21:13:55 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_paths(int i, char **path_list,
			char **command_args, char **binary_path)
{
	char	*paths;

	paths = ft_strjoin(path_list[i], "/");
	*binary_path = ft_strjoin(paths, command_args[0]);
	free(paths);
}

void	free_path_and_binary_path(char **binary_path, char ***path_list)
{
	path_list_free(*path_list);
	free(*binary_path);
}

int	check_executable_in_path(char **envpc, char **command_args)
{
	char	*paths;
	char	**path_list;
	int		i;
	char	*binary_path;

	paths = envpc_get_value(envpc, "PATH");
	if (paths != NULL)
	{
		path_list = ft_split(paths, ':');
		i = 0;
		while (path_list[i])
		{
			join_paths(i, path_list, command_args, &binary_path);
			if (check_file_type(binary_path) == 1
				&& check_file_permissions(binary_path, X_OK) == 1)
			{
				free_path_and_binary_path(&binary_path, &path_list);
				return (EXECUTABLE_PATH);
			}
			free(binary_path);
			i++;
		}
		path_list_free(path_list);
	}
	return (NOT_FOUND);
}

char	*get_exe_in_path(char **envpc, char **command_args)
{
	char	*paths;
	char	**path_list;
	int		i;
	char	*binary_path;

	paths = envpc_get_value(envpc, "PATH");
	path_list = ft_split(paths, ':');
	i = 0;
	while (path_list[i])
	{
		paths = ft_strjoin(path_list[i], "/");
		binary_path = ft_strjoin(paths, command_args[0]);
		free(paths);
		if (check_file_type(binary_path) == 1
			&& check_file_permissions(binary_path, X_OK) == 1)
		{
			break ;
		}
		free(binary_path);
		i++;
	}
	path_list_free(path_list);
	return (binary_path);
}

int	check_executable(char **envpc, char **command_args)
{
	if (ft_strchr(command_args[0], '/') == NULL)
	{
		if (check_executable_in_path(envpc, command_args) == EXECUTABLE_PATH)
			return (EXECUTABLE_PATH);
		else
			return (NOT_FOUND);
	}
	if (check_file_permissions(command_args[0], F_OK) == 0)
		return (NO_SUCH_FILE_OR_DIRECTORY);
	if (check_file_type(command_args[0]) == 2)
		return (DIRECTORY);
	if (check_file_type(command_args[0]) == 1)
	{
		if (check_file_permissions(command_args[0], X_OK) == 1)
			return (EXECUTABLE);
		else
			return (PERMISSION_DENIED);
	}
	return (NOT_FOUND);
}
