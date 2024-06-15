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

/* 
find the string in envp that starts with PWD=
move a ptr past the '=' char,
then print the rest of the chars
*/
void	print_cwd(char **envp)
{
	int		i;
	char	*s;
	
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp("PWD=", envp[i], 4) == 0)
			break;
		i++;
	}	
	s = &(envp[i][0]) + 4;
	printf("%s\n", s);
}

void	print_shell_env(char **envp)
{
	int	i;
	
	i = 0;
	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

/*
malloc space for a char **arr
then strdup n char arrays into malloc~ed space
then return a free-able ptr to the malloc~ed space
*/
char	**ft_env_dup(char **envp)
{
	char	**res;
	int		i; // index of i~th char array, in an array of char arrays
	int		j; // index of j~th char, in i~th char array, in array of char arrays
	
	i = 0;
	while (envp[i] != NULL)
		i++;
	res = (char **)malloc(sizeof(char *) * (i + 1)); // for i strings + NULL
	i = 0;
	while (envp[i] != NULL)
	{
		j = 0;
		while (envp[i][j] != '\0')
			j++;
		res[i] = (char *)malloc(sizeof(char) * (j + 1)); // for j chars + '\0'
		j = 0;
		while (envp[i][j] != '\0')
		{
			res[i][j] = envp[i][j];
			j++;		
		}
		res[i][j] = '\0';
		i++;
	}
	res[i] = NULL;
	return (res);
}

// if a REPL does only one loop, is it really a REPL ?
//int	main(int ac, char *av[])
int main(int ac, char *av[], char *envp[])
{
	(void)ac;
	(void)av;
	
	char	**shell_env;
	char	*buf;
//	while ((buf = readline(">> ")) != NULL) {
	while ((buf = readline(PROMPT)) != NULL) 
	{
		if (strlen(buf) > 0) {
		 add_history(buf);
    }

	// malloc a copy of caller's/parent's env, for the current shell
    shell_env = ft_env_dup(envp);


// ### BUILT-INS, "pwd"
	if (strcmp(buf, "pwd") == 0)
		print_cwd(shell_env);
// ### BUILT-INS, "env"
	if (strcmp(buf, "env") == 0)
		print_shell_env(shell_env);
// ### BUILT-INS, "exit"    
	if (strcmp(buf, "exit") == 0)
		exit(0);

// ### HISTORY, clear history
//	if (strcmp(buf, "clear") == 0)
//		rl_clear_history();

	printf("[%s]\n", buf);
    free(buf); // because readline malloc's a new buffer every time.
    }
    
    
	return (0);
}
