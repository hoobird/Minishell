/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:23:57 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 10:05:02 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_received_signal = 0;

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	min;

	startup(envp, &min.envpc, argc, argv);
	while (1)
	{
		min.buffer = NULL;
		signal(SIGINT, handle_readline);
		signal(SIGQUIT, SIG_IGN);
		min.buffer = readline(PROMPT);
		if (check_buffer_eof(&min.buffer) == 1)
			break ;
		if (add_hist_or_bye(&min.buffer) == 1)
			continue ;
		main_update_sigint_envpc(&min.envpc);
		min.tokenlistlist = parse_input(min.buffer, &min.envpc);
		if (check_parse_fail(&min.tokenlistlist, &min.buffer, &min.envpc) == 1)
			continue ;
		min.cmdargslist = upgrade_struct_generate_pipes(min.tokenlistlist);
		free(min.tokenlistlist);
		if (perform_redirection(&min.cmdargslist, &min.envpc, &min.buffer) == 1)
			continue ;
		execution(&min.cmdargslist, &min.envpc);
		free_cmd_args_buffer(&min.cmdargslist, &min.buffer);
	}
	return (main_exit_free_envpc(&min.envpc));
}
