/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoobird <hoobird@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:52:18 by hulim             #+#    #+#             */
/*   Updated: 2024/08/25 14:26:57 by hoobird          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_in_child(t_command_args ***command_args, int index,
			char ***envpc, char ***command_args_string)
{
	int		i;
	pid_t	pid;

	pid = fork();
	(*command_args)[index]->pid = pid;
	if (pid == 0)
	{
		if ((*command_args)[index]->writefd != STDOUT_FILENO)
			dup2((*command_args)[index]->writefd, STDOUT_FILENO);
		if ((*command_args)[index]->readfd != STDIN_FILENO)
			dup2((*command_args)[index]->readfd, STDIN_FILENO);
		i = 0;
		while ((*command_args)[i])
		{
			if ((*command_args)[i]->writefd != STDOUT_FILENO)
				close((*command_args)[i]->writefd);
			if ((*command_args)[i]->readfd != STDIN_FILENO)
				close((*command_args)[i]->readfd);
			i++;
		}
		exit(run_builtin(envpc, command_args_string, command_args, NULL));
	}
}

void	execute_in_child_signal(char p_or_c)
{
	if (p_or_c == 'p')
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (p_or_c == 'c')
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}

void	close_writefd_readfd(t_command_args **command_args, int i)
{
	if (command_args[i]->writefd != STDOUT_FILENO)
		close(command_args[i]->writefd);
	if (command_args[i]->readfd != STDIN_FILENO)
		close(command_args[i]->readfd);
}

void	execute_in_child(t_command_args ***command_args, int index,
			char ***envpc, char ***cmd_args_str)
{
	int		i;

	execute_in_child_signal('p');
	(*command_args)[index]->pid = fork();
	if ((*command_args)[index]->pid == 0)
	{
		execute_in_child_signal('c');
		if ((*command_args)[index]->writefd != STDOUT_FILENO)
			dup2((*command_args)[index]->writefd, STDOUT_FILENO);
		if ((*command_args)[index]->readfd != STDIN_FILENO)
			dup2((*command_args)[index]->readfd, STDIN_FILENO);
		i = 0;
		while ((*command_args)[i])
			close_writefd_readfd((*command_args), i++);
		if (check_cmd_type(*envpc, *cmd_args_str) == EXECUTABLE_PATH)
			execve(get_exe_in_path(*envpc, *cmd_args_str), *cmd_args_str, *envpc);
		else
			execve((*cmd_args_str)[0], *cmd_args_str, *envpc);
		free(*cmd_args_str);
		free_command_args(*command_args);
		envpc_free(envpc);
		exit(0);
	}
}
