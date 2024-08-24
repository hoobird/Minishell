/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:34:45 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 08:06:04 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	args_length(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	close_all_fds(t_command_args **command_args)
{
	int	i;

	i = 0;
	while (command_args[i])
	{
		if (command_args[i]->readfd != STDIN_FILENO)
			close(command_args[i]->readfd);
		if (command_args[i]->writefd != STDOUT_FILENO)
			close(command_args[i]->writefd);
		i++;
	}
}

void	free_everything_exe(char ***status, char ***envpc,
			t_command_args ***command_args, int *fds)
{
	if (fds != NULL)
	{
		close(fds[0]);
		close(fds[1]);
	}
	close_all_fds(*command_args);
	free(*status);
	freecommandlist(command_args);
	envpc_free(envpc);
}

void	builtin_exit_string_size_not_2(char ***status, char ***envpc,
			t_command_args ***command_args, int *fds)
{
	int	i;

	if (args_length((*status)) > 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		free_everything_exe(status, envpc, command_args, fds);
		exit(1);
	}
	if (args_length((*status)) == 1)
	{
		i = ft_atoi(envpc_get_value(*envpc, "?"));
		free_everything_exe(status, envpc, command_args, fds);
		exit(i);
	}
}

void	builtin_exit_string(char ***status, char ***envpc,
			t_command_args ***command_args, int *fds)
{
	int	i;

	builtin_exit_string_size_not_2(status, envpc, command_args, fds);
	i = 0;
	if ((*status)[1][0] == '-' || (*status)[1][0] == '+')
		i++;
	while ((*status)[1][i])
	{
		if (!ft_isdigit((*status)[1][i]))
		{
			ft_putstr_fd("exit\n", 1);
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd((*status)[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
		i++;
	}
	i = ft_atoi((*status)[1]);
	free_everything_exe(status, envpc, command_args, fds);
	exit(i);
}
