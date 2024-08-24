/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:28:48 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:35:25 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// checks if its bash delimiter (|, spaces, \0, <, <<, >, >>)
// returns 0 if not delimiter, else number of spaces occpied by delimeter
int	ft_isbashdelimiter(char *pt)
{
	if (ft_strncmp(pt, ">>", 2) == 0 || ft_strncmp(pt, "<<", 2) == 0)
		return (2);
	if (*pt == '|' || *pt == ' ' || *pt == '\t' || *pt == '<' || *pt == '>'
		|| *pt == '$')
		return (1);
	return (0);
}

// return 1 if there ienvpc_add(envpc, "?", ft_itoa(outcome));s space, 
// returns 0 if theres no space
int	check_if_space(char *str)
{
	if (*str == ' ' || *str == '\t')
		return (1);
	return (0);
}

// check for redirections and pipes
t_tokentype	check_redirection_pipe_type(char *s)
{
	if (ft_strncmp(s, ">>", 2) == 0)
		return (RE_APPEND);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (RE_HEREDOC);
	if (ft_strncmp(s, ">", 1) == 0)
		return (RE_OUTPUT);
	if (ft_strncmp(s, "<", 1) == 0)
		return (RE_INPUT);
	if (ft_strncmp(s, "|", 1) == 0)
		return (PIPE);
	return (0);
}

// add token with check if its empty string or just made up of spaces
// also check if token list is empty
void	check_then_add_token(t_token **token, char *word,
	t_tokentype type, int spaces)
{
	int	i;

	if (type != WORD)
	{
		if (*token == NULL)
			*token = init_tokenlist(word, type, spaces);
		else
			add_token(*token, word, type, spaces);
		return ;
	}
	i = 0;
	while (word[i])
	{
		if (word[i] != ' ' && word[i] != '\t')
		{
			if (*token == NULL)
				*token = init_tokenlist(word, type, spaces);
			else
				add_token(*token, word, type, spaces);
			return ;
		}
		i++;
	}
	free(word);
}
