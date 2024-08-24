/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_linkedlist3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 07:47:35 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 07:58:33 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// remove specified token from token list using index position
void	remove_token(t_token **token, int index)
{
	t_token	*next;
	t_token	*delete;
	t_token	*prev;

	if (index == 0)
	{
		delete = *token;
		*token = (*token)->next;
		free(delete->string);
		free(delete);
		return ;
	}
	prev = get_token(*token, index - 1);
	delete = prev->next;
	next = delete->next;
	prev->next = next;
	free(delete->string);
	free(delete);
}

// free token list
void	free_tokenlist(t_token **token)
{
	t_token	*temp;

	while (*token != NULL)
	{
		temp = (*token)->next;
		free((*token)->string);
		free(*token);
		*token = temp;
	}
	*token = NULL;
}

// free list of token list
void	free_tokenlistlist(t_token ***tokenlist)
{
	int	i;

	i = 0;
	while ((*tokenlist)[i] != NULL)
	{
		free_tokenlist(&(*tokenlist)[i]);
		i++;
	}
	free(*tokenlist);
	*tokenlist = NULL;
}

char	*tokenstostring(t_token *token)
{
	char	*str;
	char	*temp;

	if (token == NULL)
		return (NULL);
	str = ft_strdup("");
	while (token != NULL)
	{
		temp = ft_strjoin(str, token->string);
		free(str);
		str = temp;
		token = token->next;
	}
	return (str);
}

// get token type name
char	*get_tokentype(t_tokentype type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == SQUOTE)
		return ("SQUOTE");
	else if (type == DQUOTE)
		return ("DQUOTE");
	else if (type == RE_OUTPUT)
		return ("RE_OUTPUT");
	else if (type == RE_APPEND)
		return ("RE_APPEND");
	else if (type == RE_INPUT)
		return ("RE_INPUT");
	else if (type == RE_HEREDOC)
		return ("RE_HEREDOC");
	else if (type == RE_HEREDOC_QUOTED)
		return ("RE_HEREDOC_QUOTED");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == COMMAND)
		return ("COMMAND");
	else if (type == ARGS)
		return ("ARGS");
	else if (type == ERROR_UNCLOSED_QUOTES)
		return ("ERROR_UNCLOSED_QUOTES");
	return ("UNKNOWN");
}
