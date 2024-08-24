/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing copy 2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:28:48 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:36:44 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	increment_if_append_heredoc(t_tokentype toktype, int *i)
{
	if (toktype == RE_APPEND || toktype == RE_HEREDOC)
		(*i)++;
}

void	handle_redir_pipe_if_word(t_token *tkn, t_token **revtok)
{
	int			i;
	int			start;
	t_tokentype	toktype;

	i = 0;
	start = 0;
	while (tkn->string[i])
	{
		if (check_redirection_pipe_type(&(tkn->string[i])))
		{
			if (i - start > 0)
				check_then_add_token(revtok, ft_substr(tkn->string, start, i
						- start), WORD, check_if_space(&tkn->string[i - 1]));
			toktype = check_redirection_pipe_type(&(tkn->string[i]));
			check_then_add_token(revtok, ft_strdup(""), toktype, 0);
			i++;
			increment_if_append_heredoc(toktype, &i);
			start = i;
		}
		else
			i++;
	}
	if (i - start > 0)
		check_then_add_token(revtok, ft_substr(tkn->string, start, i - start),
			WORD, tkn->postspace);
}

// handle redirections and pipe chracter
t_token	*handle_redirection_pipe(t_token *token)
{
	t_token		*revisedtoken;

	revisedtoken = NULL;
	while (token != NULL)
	{
		if (token->type == WORD)
			handle_redir_pipe_if_word(token, &revisedtoken);
		else
			check_then_add_token(&revisedtoken, ft_strdup(token->string),
				token->type, token->postspace);
		token = token->next;
	}
	return (revisedtoken);
}

// check if redirection not next to each other
// check if pipe is not next to pipe
int	check_error_redirection_pipe(t_token *token)
{
	t_token	*nexttoken;

	if (token != NULL && token-> type == PIPE)
		return (printerror_unexpected_pipetoken());
	while (token != NULL)
	{
		if (token->type == PIPE)
		{
			nexttoken = token->next;
			if (nexttoken == NULL || nexttoken->type == PIPE)
				return (printerror_unexpected_pipetoken());
		}
		else if (token->type >= RE_OUTPUT && token->type <= RE_HEREDOC)
		{
			nexttoken = token->next;
			if (nexttoken == NULL || nexttoken->type == PIPE
				|| (nexttoken->type >= RE_OUTPUT
					&& nexttoken->type <= RE_HEREDOC))
				return (printerror_unexpected_token(nexttoken));
		}
		token = token->next;
	}
	return (0);
}
