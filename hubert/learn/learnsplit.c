#include "../libft.h"
#include "../minishell.h"


int	main(int argc, char **argv, char **envp)
{
	char	**envpc;
	char	*paths;
	char	**pathlist;
	int		i;
	char	*kvpair = NULL;

	// envpc = NULL;
	// envpc_print(envpc);
	// paths = envpc_get_value(envpc, "PATH");
	// pathlist = ft_split(kvpair, '=');
	// i = 0;
	// while (pathlist[i])
	// {
	// 	printf("%s\n", pathlist[i]);
	// 	i++;
	// }
	// envpc_free(&envpc);
	printf("%s\n", ft_strjoin("hello", ""));
}