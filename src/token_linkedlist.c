/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_linkedlist.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 07:47:35 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 07:57:46 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// initialise the token list (linked list) with first token
t_token	*init_tokenlist(char *word, t_tokentype type, int spacesafter)
{
	t_token	*token;

	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (token == NULL)
	{
		printerror("Error: failed to allocate memory for token\n");
		return (NULL);
	}
	token->string = word;
	token->type = type;
	token->next = NULL;
	token->postspace = spacesafter;
	return (token);
}

// get token list length
int	tokenlist_len(t_token *token)
{
	int	len;

	len = 0;
	while (token != NULL)
	{
		len++;
		token = token->next;
	}
	return (len);
}

// get token list list length
int	tokenlistlist_len(t_token **tokenlist)
{
	int	i;

	i = 0;
	while (tokenlist[i] != NULL)
		i++;
	return (i);
}

// get last token
t_token	*get_lasttoken(t_token *token)
{
	while (token->next != NULL)
		token = token->next;
	return (token);
}
