/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_linkedlist4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 07:47:35 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 09:32:45 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// print token list
void	print_tokenlist(t_token *token)
{
	int	i;

	if (token == NULL)
	{
		printf("Token list is empty\n");
		return ;
	}
	i = 0;
	while (token != NULL)
	{
		printf("Token %d: ^%s^ [type: %s] [spacesafter: %d]\n", i,
			token->string, get_tokentype(token->type), token->postspace);
		token = token->next;
		i++;
	}
}

void	print_tokenlistlist(t_token **tokenlist)
{
	int	i;

	i = 0;
	while (tokenlist[i] != NULL)
	{
		printf("Command: %d\n", i);
		print_tokenlist(tokenlist[i]);
		i++;
	}
	if (i == 0)
		printf("Token list list is empty\n");
}

// check if token list list is empty and frees it
int	check_tll_empty_free(t_token ***tokenlist)
{
	if ((*tokenlist)[0] == NULL)
	{
		free_tokenlistlist(tokenlist);
		return (1);
	}
	return (0);
}
