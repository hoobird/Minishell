# include "minishell.h"

int	is_valid_flag_n(char *flag)
{
	int	i;

	i = 1;
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **args)
{
	int i;
	int newline;

	i = 1;
	newline = 1;
	while (args[i] && args[i][0] == '-')
	{
		if (is_valid_flag_n(args[i]))
			newline = 0;
		else
			break ;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

// // check if flag only contains 'n'
// int	check_only_n(char *flag)
// {
// 	int	i;

// 	i = 1;
// 	while (flag[i])
// 	{
// 		if (flag[i] != 'n')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// // run as per echo command
// void	builtin_echo(char *flag, char **arg)
// {
// 	int newline;
// 	int i;

// 	newline = 1;
// 	if (flag && flag[0] == '-' && ft_strchr(flag, 'n') && check_only_n(flag))
// 		newline = 0;
// 	i = 0;
// 	while (arg[i])
// 	{
// 		printf("%s", arg[i]);
// 		if (arg[i + 1])
// 			printf(" ");
// 		i++;
// 	}
// 	if (newline)
// 		printf("\n");
// }

// // test builtin_echo command
// int	main(int argc, char **argv)
// {
// 	builtin_echo(argv[1], argv + 2);
// 	return (0);
// }

