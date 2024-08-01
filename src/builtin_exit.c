#include "minishell.h"

static int	args_length(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	builtin_exit(int status)
{
	unsigned char	exit_status;

	exit_status = (unsigned char)status;
	exit(exit_status);
}

void	builtin_exit_string(char **status)
{
	int	i;

	i = 0;
	if (args_length(status) > 2)
	{
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		builtin_exit(1);
	}
	while (status[1][i])
	{
		if (!ft_isdigit(status[1][i]))
		{
			printf("exit\n");
			printf("minishell: exit: %s: numeric argument required\n", status[1]);
			builtin_exit(2);
		}
		i++;
	}
	builtin_exit(ft_atoi(status[1]));
}