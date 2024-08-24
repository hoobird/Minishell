/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing copy 3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:28:48 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:37:21 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*quoted_and_dup(int *quoted, char *str)
{
	*quoted = 1;
	return (ft_strdup(str));
}

void	add_either_quoted_heredoc(t_token **newtoken, char *combined,
			int quoted, t_token *token)
{
	if (quoted == 1)
		check_then_add_token(newtoken, combined,
			RE_HEREDOC_QUOTED, token->postspace);
	else
		check_then_add_token(newtoken, combined,
			RE_HEREDOC, token->postspace);
}

void	keep_adding_no_postspace_to_heredoc(t_token **token,
	char **combined, int *quoted)
{
	char	*tmp;

	while ((*token)->next != NULL && (*token)->postspace == 0
		&& ((*token)->next->type == WORD || (*token)->next->type == SQUOTE
			|| (*token)->next->type == DQUOTE))
	{
		if ((*token)->next->type == WORD)
			tmp = ft_strtrim((*token)->next->string, " ");
		else
			tmp = quoted_and_dup(quoted, (*token)->next->string);
		*combined = ft_strjoin(*combined, tmp);
		free(tmp);
		*token = (*token)->next;
	}
}

//join redirects with file to the right
// heredoc... but must continually joins token with postspace = 0
// until hits non-WORD, non-SQUOTE, non-DQUOTE, NULL
t_token	*joinredirects_heredoc(t_token *token)
{
	t_token	*newtoken;
	char	*combined;
	int		quoted;

	newtoken = NULL;
	while (token != NULL)
	{
		quoted = 0;
		if (token->type == RE_HEREDOC)
		{
			token = token->next;
			if (token->type == WORD)
				combined = ft_strtrim(token->string, " ");
			else
				combined = quoted_and_dup(&quoted, token->string);
			keep_adding_no_postspace_to_heredoc(&token, &combined, &quoted);
			add_either_quoted_heredoc(&newtoken, combined, quoted, token);
		}
		else
			check_then_add_token(&newtoken, ft_strdup(token->string),
				token->type, token->postspace);
		token = token->next;
	}
	return (newtoken);
}
