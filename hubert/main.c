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

// cc main.c piping.c parsing.c token_linkedlist.c printerror.c builtin_env.c expand_shell_var2.c -lreadline ../Libft/libft.a -g
// valgrind --leak-check=full --show-leak-kinds=all --suppressions=../readline.supp ./a.out
int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	
	char					**envpc;
	char					*buffer;
	t_token					**tokenlistlist;
	t_command_args			**command_args_list;

	while (1)
	{
		buffer = readline(PROMPT);
		if (!buffer)
			break ;
		if (ft_strlen(buffer) > 0)
			add_history(buffer);
		envpc = envp_copy(envp); // later then move to the top cuz i need to check if memory leak is not from not freeing envpc
		// parse input
		tokenlistlist = parse_input(buffer, envpc);
    	envpc_free(&envpc);
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
		freecommandlist(&command_args_list);

		// then handle redirections

		// then execution

		free(buffer);
	}
	return (0);
}
