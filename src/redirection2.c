/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 08:18:11 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 14:26:30 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc(int signal)
{
	ft_putchar_fd('\n', 2);
	exit(signal);
}

void	readline_heredoc(t_redirection red, char ***envpc, int *pipes)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, red.fileeof) == 0)
		{
			if (line)
				free(line);
			else
				printerror_heredoc_eof(red.fileeof);
			break ;
		}
		if (red.type == RE_HEREDOC_QUOTED)
			expanded_line = ft_strdup(line);
		else
			expanded_line = expandshellvar(line, *envpc);
		ft_putstr_fd(expanded_line, pipes[1]);
		ft_putstr_fd("\n", pipes[1]);
		free(expanded_line);
		free(line);
	}
}

// HEREDOC <<
// HEREDOC must run before all other redirections (Based on XF)
int	redirect_heredoc(t_redirection **redlist, t_redirection red, char ***envpc,
		t_command_args ***command_args)
{
	int		pipes[2];
	pid_t	pid;
	int		status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	pipe(pipes);
	pid = fork();
	if (pid == 0)
	{
		close(pipes[0]);
		signal(SIGINT, handle_heredoc);
		readline_heredoc(red, envpc, pipes);
		close(pipes[1]);
		envpc_free(envpc);
		free_command_args(*command_args);
		free_redirectionlist(redlist);
		exit(0);
	}
	waitpid(pid, &status, 0);
	close(pipes[1]);
	if (WEXITSTATUS(status) == SIGINT)
		g_received_signal = SIGINT;
	return (pipes[0]);
}

void	redirect_heredoc_first(t_redirection **redirectionlist, char ***envpc,
		t_command_args ***command_args)
{
	int	i;
	int	j;

	i = 0;
	while (redirectionlist[i])
	{
		j = 0;
		while (redirectionlist[i][j].type)
		{
			if (redirectionlist[i][j].type == RE_HEREDOC
				|| redirectionlist[i][j].type == RE_HEREDOC_QUOTED)
			{
				redirectionlist[i][j].fd = redirect_heredoc(redirectionlist,
						redirectionlist[i][j], envpc, command_args);
				if (g_received_signal == SIGINT)
				{
					redirectionlist[i][j].fd = -1;
					break ;
				}
			}
			j++;
		}
		i++;
	}
}

// check file permissions
// 0 - permission denied
// 1 - OK
// 2 - No such file or directory
// 4 - file is a directory
int	handle_output_append(t_redirection *redir)
{
	int	status;

	status = 1;
	if (check_file_permissions(redir->fileeof, F_OK) == 1)
	{
		if (check_file_type(redir->fileeof) == 2)
			return (4);
		if (check_file_permissions(redir->fileeof, W_OK) == 0)
			return (0);
	}
	if (redir->type == RE_OUTPUT)
		redir->fd = open(redir->fileeof, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == RE_APPEND)
		redir->fd = open(redir->fileeof, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (status);
}
