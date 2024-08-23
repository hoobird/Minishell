/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:52:18 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 21:16:11 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_execution	initialise_execution(t_command_args ***command_args, char ***envpc,
	int *status, int *command_type)
{
	t_execution	exe;

	exe.cmd_args = command_args;
	exe.envpc = envpc;
	exe.status = status;
	exe.command_type = command_type;
	return (exe);
}

void	reset_status_command_type(int *status, int *command_type)
{
	*status = -999;
	*command_type = 0;
}

void	execution(t_command_args ***command_args, char ***envpc)
{
	int			i;
	int			command_type;
	int			status;
	int			last_status;

	last_status = -999;
	i = 0;
	while ((*command_args)[i])
	{
		reset_status_command_type(&status, &command_type);
		if (count_commands_args((*command_args)[i]->tokenlist) == 0)
			(*command_args)[i]->cancelexec = 1;
		if ((*command_args)[i]->cancelexec == 0)
			execute_cmd(initialise_execution(command_args, envpc,
					&status, &command_type), i);
		else
			status = (*command_args)[i]->cancelexec % 2;
		execution_close_fds(command_args, i);
		if ((*command_args)[i + 1] == NULL && command_type != EXECUTABLE
			&& command_type != EXECUTABLE_PATH)
			last_status = status;
		i++;
	}
	wait_childs_update_status(command_args, &status);
	update_question_mark(envpc, status, last_status);
}
