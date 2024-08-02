#include "minishell.h"

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
	while (1)
	{
		envpc_add(&envpc, "?", "0");
		buffer = NULL;
		buffer = readline(PROMPT);
		if (!buffer)
			break ;
		if (ft_strlen(buffer) > 0)
			add_history(buffer);
		// parse input
		tokenlistlist = parse_input(buffer, envpc);
		// print_tokenlistlist(tokenlistlist);
		if (tokenlistlist == NULL || check_tokenlistlist_empty_and_free(tokenlistlist))
		{
			printf("Error: parsing failed\n");
			continue ;
		}
		// print_tokenlistlist(tokenlistlist);
		// printf("\n");
		// next is to create pipes and upgrade t_token ** to t_command_args *
		command_args_list = upgrade_struct_generate_pipes(tokenlistlist);
		free(tokenlistlist);
		// printcommandlist(command_args_list);
		// then handle redirections
		perform_redirection(command_args_list);

		// then execution
		execution(command_args_list, &envpc);

		freecommandlist(&command_args_list);
		free(buffer);
	}
	envpc_free(&envpc);
	return (0);
}
