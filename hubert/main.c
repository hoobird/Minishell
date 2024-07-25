#include "minishell.h"

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	
	char	**envpc;
	char	*buffer;
	t_token	**tokenlistlist;

	envpc = envp_copy(envp);
	while (1)
	{
		buffer = readline(PROMPT);
		if (!buffer)
			break ;
		if (ft_strlen(buffer) > 0)
			add_history(buffer);
		tokenlistlist = parse_input(buffer, envpc);
		print_tokenlistlist(tokenlistlist);
		free(buffer);
	}
    
	return (0);
}
