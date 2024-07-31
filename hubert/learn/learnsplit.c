#include "../libft.h"
#include "../minishell.h"


int	main(int argc, char **argv, char **envp)
{
	char	**envpc;
	char	*paths;
	char	**pathlist;
	int		i;

	envpc = NULL;
	envpc_print(envpc);
	paths = envpc_get_value(envpc, "PATH");
	pathlist = ft_split(paths, ':');
	i = 0;
	while (pathlist[i])
	{
		printf("%s\n", pathlist[i]);
		i++;
	}
	envpc_free(&envpc);
}