/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:52:18 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 21:20:44 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_cmd_type(char	**envpc, char **command_args)
{
	if (ft_strcmp(command_args[0], "echo") == 0)
		return (BUILTIN_ECHO);
	else if (ft_strcmp(command_args[0], "cd") == 0)
		return (BUILTIN_CD);
	else if (ft_strcmp(command_args[0], "pwd") == 0)
		return (BUILTIN_PWD);
	else if (ft_strcmp(command_args[0], "export") == 0)
		return (BUILTIN_EXPORT);
	else if (ft_strcmp(command_args[0], "unset") == 0)
		return (BUILTIN_UNSET);
	else if (ft_strcmp(command_args[0], "env") == 0)
		return (BUILTIN_ENV);
	else if (ft_strcmp(command_args[0], "exit") == 0)
		return (BUILTIN_EXIT);
	return (check_executable(envpc, command_args));
}

int	command_args_len(t_command_args **command_args)
{
	int	i;

	i = 0;
	if (command_args)
	{
		while (command_args[i])
			i++;
	}
	return (i);
}

int	run_builtin(char ***envpc, char ***command_args_string,
		t_command_args ***command_args, int *fds)
{
	int	outcome;

	outcome = 0;
	if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_ECHO)
		outcome = builtin_echo((*command_args_string));
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_CD)
		outcome = builtin_cd(&(*command_args_string)[1], envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_PWD)
		outcome = builtin_pwd(&(*command_args_string)[1], envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_EXPORT)
		outcome = builtin_export(&(*command_args_string)[1], envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_UNSET)
		outcome = builtin_unset((*command_args_string), envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_ENV)
		outcome = builtin_env((*command_args_string), envpc);
	else if (check_cmd_type(*envpc, (*command_args_string)) == BUILTIN_EXIT)
		builtin_exit_string(command_args_string, envpc, command_args, fds);
	return (outcome);
}

int	run_in_parent(t_command_args ***command_args, int index,
		char ***envpc, char ***command_args_string)
{
	int		outcome;
	int		fds[2];

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if ((*command_args)[index]->writefd != STDOUT_FILENO)
		dup2((*command_args)[index]->writefd, STDOUT_FILENO);
	if ((*command_args)[index]->readfd != STDIN_FILENO)
		dup2((*command_args)[index]->readfd, STDIN_FILENO);
	outcome = run_builtin(envpc, command_args_string, command_args, fds);
	if ((*command_args)[index]->writefd != STDOUT_FILENO)
		close((*command_args)[index]->writefd);
	if ((*command_args)[index]->readfd != STDIN_FILENO)
		close((*command_args)[index]->readfd);
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
	return (outcome);
}
