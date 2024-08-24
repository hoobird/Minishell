/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing copy 6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:28:48 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:38:27 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// the main parsing function
t_token	**parse_input(char *str, char ***envp)
{
	t_token	*tokens;
	t_token	*revisedtokens;
	t_token	**parse_output;

	tokens = process_quotes(str);
	if (check_error_process_quotes(tokens))
	{
		free_tokenlist(&tokens);
		return (NULL);
	}
	revisedtokens = retoken_word_after_expansion(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	revisedtokens = handle_redirection_pipe(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	if (check_error_redirection_pipe(tokens))
	{
		free_tokenlist(&tokens);
		return (NULL);
	}
	parse_output = parse_input_helper(tokens, envp);
	return (parse_output);
}

// parse_input_helper 
// step 4 onwards
t_token	**parse_input_helper(t_token *tokens, char ***envp)
{
	t_token	*revisedtokens;
	t_token	**parse_output;

	revisedtokens = joinredirects_heredoc(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	revisedtokens = handle_shellvars(*envp, tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	revisedtokens = joinredirects_others(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	revisedtokens = merge_stucktogether_words(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	parse_output = split_by_pipe(tokens);
	free_tokenlist(&tokens);
	label_commands_args(parse_output);
	return (parse_output);
}
