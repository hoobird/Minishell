/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:25:38 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 15:34:00 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	args_length(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	builtin_export(char **args, char ***envpc)
{
	int	i;
	int	exit;

	exit = 0;
	if (args_length(args) == 0)
	{
		printerror(
			"export: at least 1 name assignment \
expected (eg: export example=abc123)\n");
		return (1);
	}
	i = 0;
	while (args[i])
	{
		if (try_add_envvar(args[i], envpc) == 1)
			exit = 1;
		i++;
	}
	return (exit);
}

int	builtin_unset(char **args, char ***envpc)
{
	int	i;
	int	exit;

	exit = 0;
	i = 0;
	while (args[i])
	{
		if (check_if_key_legit(args[i]) == 0)
			exit = 1;
		else
			envpc_remove(envpc, args[i]);
		i++;
	}
	return (exit);
}

int	builtin_env(char **args, char ***envpc)
{
	int	i;

	if (args_length(args) > 1)
	{
		printerror("env: no options or arguments expected\n");
		return (126);
	}
	i = 0;
	while ((*envpc)[i])
	{
		if (ft_strncmp((*envpc)[i], "?=", 2) != 0)
			printf("%s\n", (*envpc)[i]);
		i++;
	}
	return (0);
}
