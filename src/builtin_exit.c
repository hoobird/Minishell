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

void	close_all_fds(t_command_args **command_args)
{
	int	i;

	i = 0;
	while (command_args[i])
	{
		if (command_args[i]->readfd != STDIN_FILENO)
			close(command_args[i]->readfd);
		if (command_args[i]->writefd != STDOUT_FILENO)
			close(command_args[i]->writefd);
		i++;
	}
}

void	free_everything_exe(char ***status, char ***envpc, t_command_args ***command_args, int* fds)
{
	if (fds != NULL)
	{
		close(fds[0]);
		close(fds[1]);
	}
	close_all_fds(*command_args);
	free(*status);
	freecommandlist(command_args);
	envpc_free(envpc);
}


void	builtin_exit_string(char ***status, char ***envpc, t_command_args ***command_args, int* fds)
{
	int	i;

	if (args_length((*status)) > 2)
	{
		ft_putstr_fd("exit\n",1);
		ft_putstr_fd("minishell: exit: too many arguments\n",2);
		free_everything_exe(status, envpc, command_args, fds);
		builtin_exit(1);
	}
	if (args_length((*status)) == 1)
	{
		i = ft_atoi(envpc_get_value(*envpc, "?"));
		free_everything_exe(status, envpc, command_args, fds);
		builtin_exit(i);
	}
	i = 0;
	if ((*status)[1][0] == '-' || (*status)[1][0] == '+')
		i++;
	while ((*status)[1][i])
	{
		if (!ft_isdigit((*status)[1][i]))
		{
			ft_putstr_fd("exit\n",1);
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd((*status)[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			builtin_exit(2);
		}
		i++;
	}
	i = ft_atoi((*status)[1]);
	// printf("i: %d\n", i);
	free_everything_exe(status, envpc, command_args, fds);
	builtin_exit(i);
}
