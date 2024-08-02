#include "includes/minishell.h"
#include "includes/libft.h"


// main method to test if ft_split works with basic string
// hello=123 into hello and 123

int main()
{
	char *str = "hello";
	char **split = ft_split(str, '=');
	int i = 0;
	while (split[i])
	{
		printf("%s\n", split[i]);
		i++;
	}
	return (0);
}