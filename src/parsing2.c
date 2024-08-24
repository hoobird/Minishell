/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing copy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:28:48 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:36:12 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// split the string into tokens normal, single_quote, double_quote
t_token	*process_quotes(char *str)
{
	t_pq_helper	helper;

	helper.quoted = 0;
	helper.i = 0;
	helper.start = 0;
	helper.token = NULL;
	while (str[helper.i])
	{
		if (str[helper.i] == '\'' || str[helper.i] == '\"')
			process_quotes_handle_in_out_quote(&helper, str);
		helper.i++;
	}
	process_quotes_add_last_word(&helper, str);
	return (helper.token);
}

//	helper function for process_quotes:
// 	add the word before the quote and the quote to the token list
void	process_quotes_handle_in_out_quote(t_pq_helper *helper, char *str)
{
	if (helper->quoted == 0)
	{
		if (helper->i - helper->start > 0)
			check_then_add_token(&(helper->token), ft_substr(str,
					helper->start, helper->i - helper->start), WORD,
				check_if_space(&str[helper->i - 1]));
		helper->quoted = str[helper->i];
		helper->start = helper->i;
	}
	else if (helper->quoted == str[helper->i])
	{
		if (helper->quoted == '\'')
			check_then_add_token(&(helper->token), ft_substr(str,
					helper->start + 1, helper->i - helper->start - 1), SQUOTE,
				check_if_space(&str[helper->i + 1]));
		else
			check_then_add_token(&(helper->token), ft_substr(str, helper->start
					+ 1, helper->i - helper->start - 1), DQUOTE,
				check_if_space(&str[helper->i + 1]));
		helper->quoted = 0;
		helper->start = helper->i + 1;
	}
}

// helper function for process_quotes:
// 		add the last word to the token list
void	process_quotes_add_last_word(t_pq_helper *helper, char *str)
{
	if (helper->quoted == 0)
	{
		if (helper->i - helper->start > 0)
			check_then_add_token(&(helper->token), ft_substr(str,
					helper->start, helper->i - helper->start), WORD,
				check_if_space(&str[helper->i - 1]));
	}
	else
	{
		check_then_add_token(&(helper->token), ft_substr(str, helper->start
				+ 1, helper->i - helper->start - 1), ERROR_UNCLOSED_QUOTES, 0);
	}
}

// check if there is unclosed quotes
int	check_error_process_quotes(t_token *token)
{
	while (token != NULL)
	{
		if (token->type == ERROR_UNCLOSED_QUOTES)
		{
			printerror("Unclosed quotes detected\n");
			return (1);
		}
		token = token->next;
	}
	return (0);
}
