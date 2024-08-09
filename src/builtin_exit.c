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
	exit(status);
}

void	builtin_exit_string(char **status, char ***envpc)
{
	int	i;

	i = 0;
	if (args_length(status) > 2)
	{
		ft_putstr_fd("exit\n",1);
		ft_putstr_fd("minishell: exit: too many arguments\n",2);
		builtin_exit(1);
	}
	if (args_length(status) == 1)
		builtin_exit(ft_atoi(envpc_get_value(*envpc, "?")));
	if (status[1][0] == '-' || status[1][0] == '+')
		i++;
	while (status[1][i])
	{
		if (!ft_isdigit(status[1][i]))
		{
			ft_putstr_fd("exit\n",1);
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(status[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			builtin_exit(2);
		}
		i++;
	}
	builtin_exit(ft_atoi(status[1]));
}
