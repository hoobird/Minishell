/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_helper3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:07:29 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 15:31:02 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freekeyvalue(char ***key_value)
{
	free((*key_value)[0]);
	free((*key_value)[1]);
	free(*key_value);
}

void	add_to_env_and_free(char ***envpc, char **key, char **value)
{
	envpc_add(envpc, *key, *value);
	free(*key);
	free(*value);
}

int	try_add_envvar(char *kvpair, char ***envpc)
{
	char	*equal;
	char	*key;
	char	*value;

	equal = ft_strchr(kvpair, '=');
	key = ft_substr(kvpair, 0, equal - kvpair);
	if (check_if_key_legit(key) == 0)
	{
		printerror("export: `");
		ft_putstr_fd(kvpair, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(key);
		return (1);
	}
	if (equal == NULL)
	{
		printerror("export: `");
		ft_putstr_fd(kvpair, 2);
		ft_putstr_fd("': assignment operator (=) expected\n", 2);
		free(key);
		return (0);
	}
	value = ft_strdup(equal + 1);
	add_to_env_and_free(envpc, &key, &value);
	return (0);
}
