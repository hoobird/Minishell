/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hulim <hulim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 20:37:51 by hulim             #+#    #+#             */
/*   Updated: 2024/06/11 23:11:40 by hulim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"



// if a REPL does only one loop, is it really a REPL ?
//int	main(int ac, char *av[])
int main(int ac, char **av)
{
	(void)ac;
	(void)av;
		
	char* buf;
//	while ((buf = readline(">> ")) != NULL) {
	while ((buf = readline(PROMPT)) != NULL) 
	{
		if (strlen(buf) > 0) {
		 add_history(buf);
    }
	if (strcmp(buf, "exit") == 0)
		exit(0);
	if (strcmp(buf, "clear") == 0)
		rl_clear_history();
	printf("[%s]\n", buf);
    free(buf); // because readline malloc's a new buffer every time.
    }
    
    
	return (0);
}