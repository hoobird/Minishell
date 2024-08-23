/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_helper2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:07:29 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 15:33:44 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// adds a key value pair to the envpc array of strings
void	envpc_add(char ***envpc, char *name, char *value)
{
	char	**newenvpc;
	int		newlength;
	int		i;
	char	**temp;

	if (envpc_find_index(*envpc, name) != -1)
	{
		temp = &(*envpc)[envpc_find_index((*envpc), name)];
		free(*temp);
		*temp = envpc_combine_key_value(name, value);
		return ;
	}
	newlength = envpc_length((*envpc)) + 1;
	newenvpc = calloc(newlength + 1, sizeof(char *));
	newenvpc[newlength - 1] = envpc_combine_key_value(name, value);
	i = 0;
	while (i < newlength - 1)
	{
		newenvpc[i] = ft_strdup((*envpc)[i]);
		i++;
	}
	envpc_free(envpc);
	*envpc = newenvpc;
}

// removes the key from the envpc array of strings
void	envpc_remove(char ***envpc, char *name)
{
	char	**newenvpc;
	int		i;
	int		j;
	int		skip;

	skip = envpc_find_index(*envpc, name);
	if (skip == -1)
		return ;
	newenvpc = calloc(envpc_length((*envpc)) + 1, sizeof(char *));
	i = 0;
	j = 0;
	while ((*envpc)[i])
	{
		if (i == skip)
		{
			i++;
			continue ;
		}
		newenvpc[j] = ft_strdup((*envpc)[i]);
		i++;
		j++;
	}
	envpc_free(envpc);
	*envpc = newenvpc;
}

// frees the envpc array of strings
void	envpc_free(char ***envpc)
{
	int	i;

	i = 0;
	while ((*envpc)[i])
	{
		free((*envpc)[i]);
		i++;
	}
	free(*envpc);
}

// prints the envpc array of strings
void	envpc_print(char **envpc)
{
	int	i;

	i = 0;
	while (envpc[i])
	{
		printf("%s\n", envpc[i]);
		i++;
	}
}

int	check_if_key_legit(char *key)
{
	int	i;

	if (key == NULL || ft_strlen(key) == 0)
		return (0);
	if (key[0] && !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
