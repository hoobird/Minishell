/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:07:29 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 15:30:28 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// copies the envp array of strings
// takes in original envp
// returns copied envp
char	**envp_copy(char **envp)
{
	int		i;
	char	**newenvp;

	newenvp = calloc(envpc_length(envp) + 1, sizeof(char *));
	i = 0;
	while (envp[i])
	{
		newenvp[i] = ft_strdup(envp[i]);
		i++;
	}
	return (newenvp);
}

// returns the length of the envp array of strings
int	envpc_length(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

// combines key and value into a single string
char	*envpc_combine_key_value(char *key, char *value)
{
	char	*temp;
	char	*combined;

	temp = ft_strjoin(key, "=");
	if (value == NULL)
		value = "";
	combined = ft_strjoin(temp, value);
	free(temp);
	return (combined);
}

// finds the index of the key in the envpc array of strings
int	envpc_find_index(char **envpc, char *name)
{
	int		i;
	char	**temp;

	i = 0;
	while (envpc[i])
	{
		temp = ft_split(envpc[i], '=');
		if (ft_strncmp(temp[0], name, ft_strlen(name) + 1) == 0)
		{
			envpc_free(&temp);
			return (i);
		}
		envpc_free(&temp);
		i++;
	}
	return (-1);
}

// gets the value of the key in the envpc array of strings
char	*envpc_get_value(char **envpc, char *name)
{
	int		i;
	char	**temp;
	char	*value;

	i = 0;
	while (envpc[i])
	{
		temp = ft_split(envpc[i], '=');
		if (ft_strncmp(temp[0], name, ft_strlen(name) + 1) == 0)
		{
			value = ft_strchr(envpc[i], '=') + 1;
			envpc_free(&temp);
			return (value);
		}
		envpc_free(&temp);
		i++;
	}
	return (NULL);
}
