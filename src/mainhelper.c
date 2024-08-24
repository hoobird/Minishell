/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainhelper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:23:57 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 10:05:21 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freecommandlist(t_command_args ***command_args_list)
{
	int	i;

	i = 0;
	while ((*command_args_list)[i])
	{
		free_tokenlist(&(*command_args_list)[i]->tokenlist);
		free((*command_args_list)[i]);
		i++;
	}
	free(*command_args_list);
	*command_args_list = NULL;
}

void	handle_readline(int sig)
{
	g_received_signal = sig;
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	is_not_empty_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

int	main_exit_free_envpc(char ***envpc)
{
	int	i;

	if (g_received_signal == SIGINT)
		envpc_add(envpc, "?", "130");
	i = ft_atoi(envpc_get_value(*envpc, "?"));
	envpc_free(envpc);
	return (i);
}

void	startup(char **envp, char ***envpc, int argc, char *argv[])
{
	char	*value;
	char	*tmp;
	char	buf[PATH_MAX];

	(void)argc;
	(void)argv;
	*envpc = envp_copy(envp);
	envpc_add(envpc, "?", "0");
	getcwd(buf, PATH_MAX);
	envpc_add(envpc, "PWD", buf);
	value = envpc_get_value(*envpc, "SHLVL");
	if (value == NULL)
		envpc_add(envpc, "SHLVL", "1");
	else
	{
		tmp = ft_itoa(ft_atoi(value) + 1);
		envpc_add(envpc, "SHLVL", tmp);
		free(tmp);
	}
}
