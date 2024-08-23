/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:23:57 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 21:49:47 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_received_signal = 0;

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

void	startup(char **envp, char ***envpc)
{
	char	*value;
	char	*tmp;
	char	buf[PATH_MAX];

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

void	main_update_sigint_envpc(char ***envpc)
{
	if (g_received_signal == SIGINT)
	{
		envpc_add(envpc, "?", "130");
		g_received_signal = 0;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	char					**envpc;
	char					*buffer;
	t_token					**tokenlistlist;
	t_command_args			**command_args_list;

	(void)argc;
	(void)argv;
	startup(envp, &envpc);
	while (1)
	{
		buffer = NULL;
		signal(SIGINT, handle_readline);
		signal(SIGQUIT, SIG_IGN);
		buffer = readline(PROMPT);
		if (buffer == NULL)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (ft_strlen(buffer) > 0 && is_not_empty_string(buffer))
			add_history(buffer);
		else
		{
			free(buffer);
			continue ;
		}
		main_update_sigint_envpc(&envpc);
		tokenlistlist = parse_input(buffer, &envpc);
		if (tokenlistlist == NULL || if_tokenlistlist_empty_free(&tokenlistlist))
		{
			free(buffer);
			envpc_add(&envpc, "?", "0");
			continue ;
		}
		command_args_list = upgrade_struct_generate_pipes(tokenlistlist);
		free(tokenlistlist);
		if (perform_redirection(&command_args_list, &envpc) == 1)
		{
			freecommandlist(&command_args_list);
			free(buffer);
			continue ;
		}
		execution(&command_args_list, &envpc);
		freecommandlist(&command_args_list);
		free(buffer);
	}
	return (main_exit_free_envpc(&envpc));
}
