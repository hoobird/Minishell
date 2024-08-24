/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_linkedlist2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 07:47:35 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 07:58:15 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// add token to the end of the token list
void	add_token(t_token *token, char *word, t_tokentype type, int spacesafter)
{
	t_token	*newtoken;

	newtoken = (t_token *)ft_calloc(1, sizeof(t_token));
	if (newtoken == NULL)
	{
		printerror("Error: failed to allocate memory for token\n");
		return ;
	}
	newtoken->string = word;
	newtoken->type = type;
	newtoken->next = NULL;
	newtoken->postspace = spacesafter;
	get_lasttoken(token)->next = newtoken;
}

// add 1 token after specified token
void	add_token_after(t_token *token, char *word,
			t_tokentype type, int spacesafter)
{
	t_token	*newtoken;

	newtoken = (t_token *)ft_calloc(1, sizeof(t_token));
	if (newtoken == NULL)
	{
		printerror("Error: failed to allocate memory for new token\n");
		return ;
	}
	newtoken->string = word;
	newtoken->type = type;
	newtoken->next = token->next;
	newtoken->postspace = spacesafter;
	token->next = newtoken;
}

// get token's index position
int	get_tokenindex(t_token *token, t_token *target)
{
	int	index;

	index = 0;
	while (token != target)
	{
		index++;
		token = token->next;
	}
	return (index);
}

// get token at specified index position
t_token	*get_token(t_token *token, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		token = token->next;
		i++;
	}
	return (token);
}

int	search_token(t_token *token, char *word)
{
	int	index;

	index = 0;
	while (token != NULL)
	{
		if (ft_strcmp(token->string, word) == 0)
			return (index);
		token = token->next;
		index++;
	}
	return (-1);
}
