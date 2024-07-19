# include "minishell.h"
# include "builtin.h"

/*
cc -Wall -Wextra -Werror expand_shell_var.c builtin_env.c ../Libft/libft.a

NOTE 1.
expand_shell_var() has two versions!
v1 does NOT enclose the returned shell variable's value with quotes
v2 returns the shell variable's value, and encloses it with quotes

eg.
expand_shell_var_v? / input / output
v1 / echo $USER$HOME   abcdef / echo "jsu"$HOME   abcdef
v2 / echo "$USER"$HOME   abcdef / echo "jsu"$HOME   abcdef

NOTE 2.
expand_dollar_sign() expands one shell variable, per call.

The function searches for 2 things: 
-> the first '$' char from the start of the input string,
-> and the first delimiter char, following the dollar sign.

Using pointer arithmetic, the input string is split into 3 parts:
part 1, the part before the shell var
part 2, the shell var, eg. $HOME
part 3, the part after the shell var
Note. all whitespace is preserved!

Part 2, is replaced with the actual expanded value, from the char **env
and the 3 parts are strjoined, and returned

Note. the caller has to free the ptr returned by expand_shell_var_vx
*/

int	is_delim_shellvar(char x)
{
	if (x == '$')
		return (1);
	if (x == '?')
		return (1);
	if (x == ' ')
		return (1);
	if (x == '\0')
		return (1);

	if (x == 39) // single quote
		return (1);
	if (x == 34) // double quote
		return (1);
	if (x == '|')
		return (1);
	if (x == '>')
		return (1);
	if (x == '<')
		return (1);

	else
		return (0);
}

/*
This helper function finds the shell variable's last ch(ar), 
by first locating the delimiter char*

* see helper function, is_delim_shellvar() above

It either returns a pointer to the last char OR it returns NULL
*/
static char	*find_last_ch(char *s)
{
	int		i;
	
	i = 0;
	while(s[i] != '\0')
	{
		if ( is_delim_shellvar(s[i]) )
			return ( &(s[i - 1]) );
		i++;
	}
	return (NULL);
}

static void	ft_strncpy(char *src, char *dst, int len)
{
	int i;
	
	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

// v1 does NOT enclose the returned shell variable's value with quotes
char	*expand_dollar_sign_v1(char *s, char **env_ptr)
{
	char	*dollar;
	char	*last_ch;
	char	*expanded;
	int		len_part1;
	int		len_part2;
	int 	len_part3;
	char	*part1;
	char	*part2;
	char	*part3;
	char	*joined;
	
	part1 = NULL;
	part2 = NULL;
	part3 = NULL;	
	
// PART 1 of 3
	dollar = ft_strchr(s, '$');
	if (!dollar)
		return (s);
	
	len_part1 = dollar - s;
	part1 = malloc(sizeof(char) * (len_part1 + 1)); // remb to free part1
	//part1 = malloc(sizeof(char) * (len_part1 + 1) + 1); // +1 for LEADING dbl quote
	if (!part1)
		part1 = NULL;
	ft_strncpy(s, part1, len_part1);
	//ft_memset(part1 + len_part1, 34, 1); // set LAST char in part1, to dbl quote char
	printf("part1: %s\n", part1);

// PART 2 of 3
	last_ch = find_last_ch(dollar + 1);
	if (last_ch == NULL)
		len_part2 = strlen(s) - len_part1;
	else
		len_part2 = (last_ch - dollar);	
	
	part2 = malloc(sizeof(char) * (len_part2 + 1)); // remb to free part2 
	if (!part2)
		part2 = NULL;
	ft_strncpy(dollar + 1, part2, len_part2);
	printf("part2: %s\n", part2);

	expanded = envpc_get_value(env_ptr, part2);
	free(part2);

// PART 3 of 3
	len_part3 = strlen(s) - len_part1 - len_part2;
	part3 = malloc(sizeof(char) * (len_part3 + 1)); // remb to free part3
	//part3 = malloc(sizeof(char) * (len_part3 + 1) + 1); // +1 for TRAILING dbl quote
	if (!part3)
		part3 = NULL;
	//ft_memset(part3, 34, 1); // set FIRST char in part3, to dbl quote char
	ft_strncpy(last_ch + 1, part3, len_part3);
	printf("part3: %s\n", part3);

// join parts 1, 2, & 3	
	char	*temp;
	temp = ft_strjoin(part1, expanded); // remb to free temp
	free(part1);
	
	joined = ft_strjoin(temp, part3); // NOTE! func. caller has to free joined
	free(part3);
	free(temp);
	
	return (joined);
}

// v2 encloses returned shell var value with quotes
char	*expand_dollar_sign_v2(char *s, char **env_ptr)
{
	char	*dollar;
	char	*last_ch;
	char	*expanded;
	int		len_part1;
	int		len_part2;
	int 	len_part3;
	char	*part1;
	char	*part2;
	char	*part3;
	char	*joined;
	
	part1 = NULL;
	part2 = NULL;
	part3 = NULL;	
	
// PART 1 of 3
	dollar = ft_strchr(s, '$');
	if (!dollar)
		return (s);
	
	len_part1 = dollar - s;
	//part1 = malloc(sizeof(char) * (len_part1 + 1)); // remb to free part1
	part1 = malloc(sizeof(char) * (len_part1 + 1) + 1); // +1 for LEADING dbl quote
	if (!part1)
		part1 = NULL;
	ft_strncpy(s, part1, len_part1);
	ft_memset(part1 + len_part1, 34, 1); // set LAST char in part1, to dbl quote char
	printf("part1: %s\n", part1);

// PART 2 of 3
	last_ch = find_last_ch(dollar + 1);
	if (last_ch == NULL)
		len_part2 = strlen(s) - len_part1;
	else
		len_part2 = (last_ch - dollar);	
	
	part2 = malloc(sizeof(char) * (len_part2 + 1)); // remb to free part2 
	if (!part2)
		part2 = NULL;
	ft_strncpy(dollar + 1, part2, len_part2);
	printf("part2: %s\n", part2);

	expanded = envpc_get_value(env_ptr, part2);
	free(part2);

// PART 3 of 3
	len_part3 = strlen(s) - len_part1 - len_part2;
	//part3 = malloc(sizeof(char) * (len_part3 + 1)); // remb to free part3
	part3 = malloc(sizeof(char) * (len_part3 + 1) + 1); // +1 for TRAILING dbl quote
	if (!part3)
		part3 = NULL;
	ft_memset(part3, 34, 1); // set FIRST char in part3, to dbl quote char
	ft_strncpy(last_ch + 1, part3 + 1, len_part3);
	printf("part3: %s\n", part3);

// join parts 1, 2, & 3	
	char	*temp;
	temp = ft_strjoin(part1, expanded); // remb to free temp
	free(part1);
	
	joined = ft_strjoin(temp, part3); // NOTE! func. caller has to free joined
	free(part3);
	free(temp);
	
	return (joined);
}

// for testing purposes only
int	main(int ac, char **av, char** env)
{
	char	*input_1;
	char	*input_2;
	char	*output_1;
	char	*output_2;
	
	(void)ac;
	(void)av;
	input_1 = "echo $USER$HOME abcdef"; 	// echo "jsu"$HOME abcdef, call v2
	input_2 = "echo \"$USER\"$HOME abcdef";	// echo "jsu"$HOME abcdef, call v1
	
	output_1 = expand_dollar_sign_v2(input_1, env);
	output_2 = expand_dollar_sign_v1(input_2, env);
	
	printf("\nbef: %s\naft: %s\n", input_1, output_1);
	printf("\nbef: %s\naft: %s\n", input_2, output_2);
// remember, the thing that calls expand_dollar_sign(), has to free the return value!
	free(output_1);
	free(output_2); 
	return (0);
}
