/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 08:18:11 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 14:26:01 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_redir(t_command_args *command_args)
{
	int		count;
	t_token	*tokens;

	count = 0;
	tokens = command_args->tokenlist;
	while (tokens)
	{
		if (tokens->type == RE_OUTPUT || tokens->type == RE_APPEND
			|| tokens->type == RE_INPUT || tokens->type == RE_HEREDOC
			|| tokens->type == RE_HEREDOC_QUOTED)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

void	add_to_redir_list(t_redirection *redir, t_token *tokens)
{
	redir->type = tokens->type;
	redir->fileeof = ft_strdup(tokens->string);
	redir->fd = -1;
}

t_redirection	**setup_redirectionlist(t_command_args **cmd_arg)
{
	int				i;
	int				j;
	t_token			*tokens;
	t_redirection	**redlst;

	redlst = ft_calloc(sizeof(t_redirection *), (command_args_len(cmd_arg)
				+ 1));
	i = -1;
	while (cmd_arg[++i])
	{
		tokens = cmd_arg[i]->tokenlist;
		redlst[i] = ft_calloc(sizeof(t_redirection), (count_redir(cmd_arg[i])
					+ 1));
		j = 0;
		while (tokens)
		{
			if (tokens->type >= RE_OUTPUT && tokens->type <= RE_HEREDOC_QUOTED)
			{
				add_to_redir_list(&redlst[i][j], tokens);
				j++;
			}
			tokens = tokens->next;
		}
	}
	return (redlst);
}

void	free_redirectionlist(t_redirection **redirectionlist)
{
	int	i;
	int	j;

	i = 0;
	while (redirectionlist[i])
	{
		j = 0;
		while (redirectionlist[i][j].type)
		{
			free(redirectionlist[i][j].fileeof);
			j++;
		}
		free(redirectionlist[i]);
		i++;
	}
	free(redirectionlist);
}
