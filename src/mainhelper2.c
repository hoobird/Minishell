/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainhelper2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:23:57 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 10:05:31 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_update_sigint_envpc(char ***envpc)
{
	if (g_received_signal == SIGINT)
	{
		envpc_add(envpc, "?", "130");
		g_received_signal = 0;
	}
}

void	free_cmd_args_buffer(t_command_args ***command_args_list, char **buffer)
{
	freecommandlist(command_args_list);
	free(*buffer);
}

int	add_hist_or_bye(char **buffer)
{
	if (is_not_empty_string(*buffer))
	{
		add_history(*buffer);
		return (0);
	}
	free(*buffer);
	return (1);
}

int	check_parse_fail(t_token ***tokenlistlist, char **buffer, char ***envpc)
{
	if (*tokenlistlist == NULL || check_tll_empty_free(tokenlistlist))
	{
		free(*buffer);
		envpc_add(envpc, "?", "0");
		return (1);
	}
	return (0);
}

int	check_buffer_eof(char **buffer)
{
	if (*buffer == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	return (0);
}
