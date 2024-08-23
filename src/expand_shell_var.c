/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_shell_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:21:41 by hulim             #+#    #+#             */
/*   Updated: 2024/08/23 21:23:41 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_envvar_delimiter(char *start)
{
	int	i;

	if (*start != '$')
		return (0);
	if (start[1] == '?')
		return (2);
	if (!ft_isalpha(start[1]) && start[1] != '_')
		return (1);
	i = 1;
	while (ft_isalnum(start[i]) || start[i] == '_')
		i++;
	return (i);
}

char	*shell_expand_append(char **envp, char **str, char *input, int len)
{
	char	*temp;
	char	*key;
	char	*value;

	if (*input == '$' && len > 1)
	{
		key = ft_substr(input, 1, len - 1);
		value = envpc_get_value(envp, key);
		if (value)
			temp = ft_strjoin(*str, value);
		else
			temp = ft_strjoin(*str, "");
	}
	else
	{
		key = ft_substr(input, 0, len);
		temp = ft_strjoin(*str, key);
	}
	free(*str);
	free(key);
	return (temp);
}

char	*expandshellvar(char *input, char **envp)
{
	int		i;
	int		shellvar_len;
	char	*output;

	i = 0;
	output = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$')
		{
			shellvar_len = check_envvar_delimiter(&input[i]);
			if (shellvar_len == 1)
				output = shell_expand_append(envp, &output, &input[i], 1);
			else
				output = shell_expand_append(envp, &output,
						&input[i], shellvar_len);
			i += shellvar_len;
		}
		else
		{
			output = shell_expand_append(envp, &output, &input[i], 1);
			i++;
		}
	}
	return (output);
}
