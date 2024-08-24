/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing copy 4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:28:48 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:37:59 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_word_dup_others(t_token *nexttoken)
{
	if (nexttoken->type == WORD)
		return (ft_strtrim(nexttoken->string, " "));
	return (ft_strdup(nexttoken->string));
}

//join redirects with file to the right
// for output, append, input (NOT HEREDOC - done earlier)
t_token	*joinredirects_others(t_token *token)
{
	t_token	*newtoken;
	t_token	*nexttoken;
	char	*combined;
	char	*tmp;

	newtoken = NULL;
	while (token != NULL)
	{
		if (token->type >= RE_OUTPUT && token->type <= RE_INPUT)
		{
			nexttoken = token->next;
			tmp = trim_word_dup_others(nexttoken);
			combined = ft_strjoin(token->string, tmp);
			free(tmp);
			check_then_add_token(&newtoken, combined,
				token->type, nexttoken->postspace);
			token = nexttoken;
		}
		else
			check_then_add_token(&newtoken, ft_strdup(token->string),
				token->type, token->postspace);
		token = token->next;
	}
	return (newtoken);
}

// expand shell vars
t_token	*handle_shellvars(char **envp, t_token *token)
{
	t_token	*newtoken;
	char	*expanded;

	newtoken = NULL;
	while (token != NULL)
	{
		if (token->type == WORD || token->type == DQUOTE)
		{
			expanded = expandshellvar(token->string, envp);
			check_then_add_token(&newtoken, expanded, token->type,
				token->postspace);
		}
		else
			check_then_add_token(&newtoken, ft_strdup(token->string),
				token->type, token->postspace);
		token = token->next;
	}
	return (newtoken);
}

// retokenise the WORD token after expansion
t_token	*retoken_word_after_expansion(t_token *etokens)
{
	t_token	*ntoken;
	char	**splitted;
	int		i;

	ntoken = NULL;
	while (etokens != NULL)
	{
		if (etokens->type == WORD)
		{
			splitted = ft_split(etokens->string, ' ');
			i = 0;
			while (splitted[i])
			{
				check_then_add_token(&ntoken, ft_strdup(splitted[i]), WORD, 1);
				free(splitted[i++]);
			}
			free(splitted);
			get_lasttoken(ntoken)->postspace = etokens->postspace;
		}
		else
			check_then_add_token(&ntoken, ft_strdup(etokens->string),
				etokens->type, etokens->postspace);
		etokens = etokens->next;
	}
	return (ntoken);
}
