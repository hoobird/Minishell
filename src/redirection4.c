/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 08:18:11 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 14:28:41 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assignreadwritefd(t_command_args **cmd_args, t_redirection **rl)
{
	int	i;
	int	j;
	int	original_readfd;
	int	original_writefd;

	i = 0;
	while (cmd_args[i])
	{
		j = 0;
		original_readfd = cmd_args[i]->readfd;
		original_writefd = cmd_args[i]->writefd;
		while (rl[i][j].fd)
		{
			if (rl[i][j].fd == -1)
			{
				j++;
				continue ;
			}
			update_fds(rl, i, j, cmd_args);
			j++;
		}
		close_original_fds(original_readfd, cmd_args, i, original_writefd);
		i++;
	}
}

void	closeunusedfd(t_redirection **redirlist, t_command_args **command_args)
{
	int	i;
	int	j;

	i = 0;
	while (redirlist[i])
	{
		j = 0;
		while (redirlist[i][j].type)
		{
			if (redirlist[i][j].fd != -1
				&& redirlist[i][j].fd != command_args[i]->readfd
				&& redirlist[i][j].fd != command_args[i]->writefd)
				close(redirlist[i][j].fd);
			j++;
		}
		i++;
	}
}

void	cancel_all_exec(t_command_args **command_args)
{
	int	i;

	i = 0;
	while (command_args[i])
	{
		command_args[i]->cancelexec = 1;
		i++;
	}
}

int	perform_redirection(t_command_args ***command_args, char ***envpc,
		char **buffer)
{
	t_redirection	**redirectionlist;
	int				siginted;

	siginted = 0;
	redirectionlist = setup_redirectionlist(*command_args);
	redirect_heredoc_first(redirectionlist, envpc, command_args);
	redirect_rest_later(redirectionlist, *command_args);
	assignreadwritefd(*command_args, redirectionlist);
	closeunusedfd(redirectionlist, *command_args);
	free_redirectionlist(redirectionlist);
	if (g_received_signal == SIGINT)
	{
		siginted = 1;
		envpc_add(envpc, "?", "130");
		g_received_signal = 0;
		free_cmd_args_buffer(command_args, buffer);
	}
	return (siginted);
}
