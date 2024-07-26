#include "minishell.h"

// cc main.c piping.c parsing.c token_linkedlist.c printerror.c builtin_env.c expand_shell_var2.c -lreadline ../Libft/libft.a -g
// valgrind --leak-check=full --show-leak-kinds=all --suppressions=../readline.supp ./a.out
int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	
	char					**envpc;
	char					*buffer;
	t_command_life_cycle	command_life_cycle;

	while (1)
	{
		buffer = readline(PROMPT);
		if (!buffer)
			break ;
		if (ft_strlen(buffer) > 0)
			add_history(buffer);
		envpc = envp_copy(envp); // later then move to the top cuz i need to check if memory leak is not from not freeing envpc
		// parse input
		command_life_cycle.tokenlistlist = parse_input(buffer, envpc);
    	envpc_free(&envpc);
		if (command_life_cycle.tokenlistlist == NULL || check_tokenlistlist_empty_and_free(command_life_cycle.tokenlistlist))
		{
			// printf("Error: parsing failed\n");
			continue ;
		}
		// print_tokenlistlist(tokenlistlist);
		// next is to create pipes
		command_life_cycle.pipefd = generatepipes(tokenlistlist_len(command_life_cycle.tokenlistlist) - 1);
		// printpipelist(command_life_cycle.pipefd);
		// then handle redirections

		// then execution

		free(buffer);
	}
	return (0);
}
