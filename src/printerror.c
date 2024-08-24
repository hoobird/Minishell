/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printerror.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 08:07:21 by hulim             #+#    #+#             */
/*   Updated: 2024/08/24 17:30:22 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// error ive seen so far
// minishell: a: command not found
// minishell: syntax error near unexpected token `>'
// minishell: Command 'cata' not found
int	printerror(char	*errormsg)
{
	char	*temp;

	temp = ft_strjoin("minishell: ", errormsg);
	ft_putstr_fd(temp, 2);
	free(temp);
	return (1);
}

void	printerror_heredoc_eof(char *eof)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(eof, 2);
	ft_putstr_fd("')\n", 2);
}

int	printerror_unexpected_pipetoken(void)
{
	printerror("Syntax error near unexpected token `|'\n");
	return (1);
}

int	printerror_unexpected_token(t_token *nexttoken)
{
	printerror("Syntax error near unexpected token `");
	if (nexttoken != NULL)
		ft_putstr_fd(nexttoken->string, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}
