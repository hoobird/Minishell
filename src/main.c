#include "minishell.h"

int g_received_signal = 0;

void	freecommandlist(t_command_args ***command_args_list)
{
	int i;

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
		rl_on_new_line(); // move cursor to the beginning of the line
		rl_replace_line("", 0); // clear the line
		rl_redisplay(); // redisplay the line
	}
}

	

// cc main.c builtin_cd_pwd.c  builtin_echo.c builtin_exit.c  execute.c redirection.c piping.c parsing.c token_linkedlist.c printerror.c builtin_env.c expand_shell_var.c check_file_status.c -lreadline ../Libft/libft.a -g
// valgrind --leak-check=full --show-leak-kinds=all --suppressions=../readline.supp ./a.out
// valgrind --leak-check=full --show-leak-kinds=all --suppressions=../readline.supp --track-fds=yes --trace-children=yes ./a.out 
int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	
	char					**envpc;
	char					*buffer;
	t_token					**tokenlistlist;
	t_command_args			**command_args_list;

	envpc = envp_copy(envp); // later then move to the top cuz i need to check if memory leak is not from not freeing envpc
	envpc_add(&envpc, "?", "0");
	while (1)
	{
		buffer = NULL;
		signal(SIGINT, handle_readline); // ctrl + c
		signal(SIGQUIT, SIG_IGN); // ctrl + slash
		buffer = readline(PROMPT);
		if (buffer == NULL)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (ft_strlen(buffer) > 0)
			add_history(buffer);
		if (g_received_signal == SIGINT)
			envpc_add(&envpc, "?", "130");
		// parse input
		tokenlistlist = parse_input(buffer, envpc);
		// print_tokenlistlist(tokenlistlist);
		if (tokenlistlist == NULL || check_tokenlistlist_empty_and_free(tokenlistlist))
		{
			// printf("Error: parsing failed\n");
			continue ;
		}
		// print_tokenlistlist(tokenlistlist);
		// printf("\n");
		// next is to create pipes and upgrade t_token ** to t_command_args *
		command_args_list = upgrade_struct_generate_pipes(tokenlistlist);
		free(tokenlistlist);
		// printcommandlist(command_args_list);
		// then handle redirections
		perform_redirection(command_args_list, &envpc);
		if (g_received_signal == SIGINT)
		{
			freecommandlist(&command_args_list);
			free(buffer);
			envpc_add(&envpc, "?", "130");
			g_received_signal = 0;
			continue ;
		}
		// printcommandlist(command_args_list);
		// then execution
		execution(command_args_list, &envpc);

		freecommandlist(&command_args_list);
		free(buffer);
		g_received_signal = 0;
	}
	envpc_free(&envpc);
	return (0);
}
