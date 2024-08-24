/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing copy 5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:28:48 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:38:34 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	keep_merging_no_postspace_to_token(t_token **etokens, char **combined)
{
	char	*tmp;

	while ((*etokens)->next != NULL && (*etokens)->postspace == 0
		&& ((*etokens)->next->type >= WORD && (*etokens)->next->type <= DQUOTE))
	{
		tmp = ft_strjoin(*combined, (*etokens)->next->string);
		free(*combined);
		*combined = tmp;
		*etokens = (*etokens)->next;
	}
}

// merge together words that are stuck together
t_token	*merge_stucktogether_words(t_token *etokens)
{
	t_token	*newtoken;
	t_token	*tmp;
	char	*combined;

	newtoken = NULL;
	while (etokens != NULL)
	{
		tmp = etokens;
		if (etokens->type != PIPE && etokens->postspace == 0)
		{
			combined = ft_strdup(etokens->string);
			keep_merging_no_postspace_to_token(&etokens, &combined);
			check_then_add_token(&newtoken, combined, tmp->type,
				tmp->postspace);
		}
		else
			check_then_add_token(&newtoken, ft_strdup(etokens->string),
				etokens->type, etokens->postspace);
		etokens = etokens->next;
	}
	if (etokens != NULL)
		check_then_add_token(&newtoken, ft_strdup(etokens->string),
			etokens->type, etokens->postspace);
	return (newtoken);
}

// count number of pipes in token list
int	count_pipes(t_token *token)
{
	int		count;

	count = 0;
	while (token != NULL)
	{
		if (token->type == PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

// split by pipe
// returns a list of token list
t_token	**split_by_pipe(t_token *token)
{
	t_token	**newtokens;
	int		i;

	newtokens = ft_calloc(count_pipes(token) + 2, sizeof(t_token *));
	i = 0;
	while (token != NULL)
	{
		if (token->type == PIPE)
		{
			i++;
			token = token->next;
		}
		else
		{
			if (newtokens[i] == NULL)
				newtokens[i] = init_tokenlist(ft_strdup(token->string),
						token->type, token->postspace);
			else
				add_token(newtokens[i], ft_strdup(token->string), token->type,
					token->postspace);
			token = token->next;
		}
	}
	return (newtokens);
}

// label commands and args now
// 1. if first token is a WORD or SQUOTE or DQUOTE, then its a command
// 2. if first token is a RE_OUTPUT, RE_APPEND, RE_INPUT, RE_HEREDOC, 
// then move down the list until you find a WORD, SQUOTE, DQUOTE,
// then its a command
// FINALLY, AFTER COMMAND IS APPOINTED, FOLLOWING WORD SQUOTE DQUOTE ARE ARGS
// DONT NEED TO LABEL REDIRECTIONS
void	label_commands_args(t_token **tokenlistlist)
{
	int		i;
	t_token	*tokens;
	int		commandfound;

	i = 0;
	while (tokenlistlist[i] != NULL)
	{
		commandfound = 0;
		tokens = tokenlistlist[i];
		while (tokens != NULL)
		{
			if (commandfound == 0 && (tokens->type >= WORD
					&& tokens->type <= DQUOTE))
			{
				tokens->type = COMMAND;
				commandfound = 1;
			}
			else
				if (tokens->type >= WORD && tokens->type <= DQUOTE)
					tokens->type = ARGS;
			tokens = tokens->next;
		}
		i++;
	}
}
