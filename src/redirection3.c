/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 08:18:11 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 14:26:54 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input(t_redirection *redir)
{
	int	status;

	status = 1;
	if (check_file_permissions(redir->fileeof, F_OK) == 0)
		return (2);
	if (check_file_type(redir->fileeof) == 2)
		return (4);
	if (check_file_permissions(redir->fileeof, R_OK) == 0)
		return (0);
	if (status == 1)
		redir->fd = open(redir->fileeof, O_RDONLY);
	return (status);
}

void	redir_print_fail(int status, char *fileeof)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(fileeof, 2);
	if (status == 0)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (status == 2)
		ft_putstr_fd(": No such file or directory\n", 2);
	else if (status == 4)
		ft_putstr_fd(": Is a directory\n", 2);
}

void	redirect_rest_later(t_redirection **rl, t_command_args **command_args)
{
	int	i;
	int	j;
	int	status;

	i = 0;
	status = 1;
	while (rl[i])
	{
		j = 0;
		while (rl[i][j].type)
		{
			if (rl[i][j].type == RE_OUTPUT || rl[i][j].type == RE_APPEND)
				status = handle_output_append(&rl[i][j]);
			else if (rl[i][j].type == RE_INPUT)
				status = handle_input(&rl[i][j]);
			if (status == 0 || status == 2 || status == 4)
			{
				redir_print_fail(status, rl[i][j].fileeof);
				command_args[i]->cancelexec = 1;
				break ;
			}
			j++;
		}
		i++;
	}
}

void	update_fds(t_redirection **rl, int i, int j, t_command_args **cmd_args)
{
	if (rl[i][j].type == RE_OUTPUT || rl[i][j].type == RE_APPEND)
		cmd_args[i]->writefd = rl[i][j].fd;
	else if (rl[i][j].type == RE_INPUT || rl[i][j].type == RE_HEREDOC
		|| rl[i][j].type == RE_HEREDOC_QUOTED)
		cmd_args[i]->readfd = rl[i][j].fd;
}

void	close_original_fds(int original_readfd, t_command_args **cmd_args,
		int i, int original_writefd)
{
	if (original_readfd != 0 && original_readfd != cmd_args[i]->readfd)
		close(original_readfd);
	if (original_writefd != 1 && original_writefd != cmd_args[i]->writefd)
		close(original_writefd);
}
