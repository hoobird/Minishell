/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoobird <hoobird@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:52:18 by hulim             #+#    #+#             */
/*   Updated: 2024/08/25 14:24:50 by hoobird          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_question_mark(char ***envpc, int status, int last_status)
{
	char	*exitstring;

	if (status == 130)
	{
		exitstring = ft_itoa(130);
	}
	else if (status == 131)
	{
		exitstring = ft_itoa(131);
		ft_putstr_fd("Quit\n", 1);
	}
	else if (last_status != -999)
	{
		exitstring = ft_itoa(last_status);
	}
	else
	{
		exitstring = ft_itoa(status);
	}
	envpc_add(envpc, "?", exitstring);
	free(exitstring);
}

void	handle_command_errors(int *status, int command_type, char **cmd_arg_str)
{
	if (command_type >= DIRECTORY && command_type <= NO_SUCH_FILE_OR_DIRECTORY)
		ft_putstr_fd("minishell: ", 2);
	if ((command_type >= DIRECTORY && command_type
			<= NO_SUCH_FILE_OR_DIRECTORY) || command_type == NOT_FOUND)
		ft_putstr_fd(cmd_arg_str[0], 2);
	if (command_type == DIRECTORY)
		ft_putstr_fd(": Is a directory\n", 2);
	else if (command_type == PERMISSION_DENIED)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (command_type == NOT_FOUND)
		ft_putstr_fd(": command not found\n", 2);
	else if (command_type == NO_SUCH_FILE_OR_DIRECTORY)
		ft_putstr_fd(": No such file or directory\n", 2);
	if (command_type == DIRECTORY || command_type == PERMISSION_DENIED)
		*status = 126;
	else if (command_type == NOT_FOUND
		|| command_type == NO_SUCH_FILE_OR_DIRECTORY)
		*status = 127;
}

void	execution_close_fds(t_command_args ***command_args, int i)
{
	if ((*command_args)[i]->writefd != STDOUT_FILENO)
	{
		close((*command_args)[i]->writefd);
		(*command_args)[i]->writefd = STDOUT_FILENO;
	}
	if ((*command_args)[i]->readfd != STDIN_FILENO)
	{
		close((*command_args)[i]->readfd);
		(*command_args)[i]->readfd = STDIN_FILENO;
	}
}

void	wait_childs_update_status(t_command_args ***command_args, int *status)
{
	int		i;

	i = 0;
	while ((*command_args)[i])
	{
		waitpid((*command_args)[i]->pid, status, 0);
		if (WIFSIGNALED(*status))
			if (WTERMSIG(*status) == SIGINT)
				ft_putstr_fd("\n", 1);
		i++;
	}
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		*status = WTERMSIG(*status) + 128;
}

void	execute_cmd(t_execution exe, int i)
{
	char	**cmd_args_str;

	cmd_args_str = command_args_extraction((*exe.cmd_args)[i]->tokenlist);
	*exe.command_type = check_cmd_type(*exe.envpc, cmd_args_str);
	if (*exe.command_type < 98)
	{
		if (command_args_len((*exe.cmd_args)) == 1)
			*exe.status = run_in_parent(exe.cmd_args, i,
					exe.envpc, &cmd_args_str);
		else
			run_in_child(exe.cmd_args, i, exe.envpc, &cmd_args_str);
	}
	else if (*exe.command_type == EXECUTABLE
		|| *exe.command_type == EXECUTABLE_PATH)
		execute_in_child(exe.cmd_args, i, exe.envpc, &cmd_args_str);
	else
		handle_command_errors(exe.status, *exe.command_type, cmd_args_str);
	free(cmd_args_str);
}
