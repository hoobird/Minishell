# include "minishell.h"
// #include "../includes/minishell.h"

char **tokenise(char *buf);
char **add_to_strarr(char **oldtokenarr, char *token);
int get_strarr_len(char **tokenarr);
int mini_isspace(char c);
int	mini_isempty(char	*str);
int ft_isbashdelimiter(char *pt);
int ft_isquote(char pt);

// for testing purpose
void print_str_arr(char **strarr);

// Tokenise a string
// cat -e "hello" | ls --> {"cat", "-e", "hello", "|", "ls"}
char **tokenise(char *buf)
{
	char **tokenarr;
	char *tokencurrent;
	int quoteopen;
	int index;
	int start;

	start = 0;
	index = 0;
	quoteopen = 0;
	start = index;
	tokenarr = calloc(sizeof(char *), 1);
	if (!tokenarr)
		return (NULL);
	while (buf[index] && mini_isspace(buf[index]))
		index++;
	while (buf[index])
	{
		if (buf[index + 1] == 0)
		{
			tokencurrent = ft_substr(buf, start, index - start + 1);
			tokenarr = add_to_strarr(tokenarr, tokencurrent);
			free(tokencurrent);
			break;
		}
		if (quoteopen > 0)
		{
			if (ft_isquote(buf[index]) == quoteopen)
			{
				quoteopen = 0;
				tokencurrent = ft_substr(buf, start, index - start + 1);
				tokenarr = add_to_strarr(tokenarr, tokencurrent);
				free(tokencurrent);
				start = index + 1;
			}
			else
			{
				index++;
				continue;
			}
		}
		else if (ft_isquote(buf[index]))
		{
			quoteopen = ft_isquote(buf[index]);
			start = index;
			index++;
			continue;
		}
		else if (buf[index] == '$')
		{
			tokencurrent = ft_substr(buf, start, index - start);
			tokenarr = add_to_strarr(tokenarr, tokencurrent);
			free(tokencurrent);
			start = index;
			while (buf[index] && ft_isbashdelimiter(&(buf[index])) == 0)
				index++;
			tokencurrent = ft_substr(buf, start, index - start);
			tokenarr = add_to_strarr(tokenarr, tokencurrent);
			free(tokencurrent);
			start = index;
		}
		else if (ft_isbashdelimiter(&(buf[index])))
		{
			tokencurrent = ft_substr(buf, start, index - start);
			tokenarr = add_to_strarr(tokenarr, tokencurrent);
			free(tokencurrent);
			if (ft_isbashdelimiter(&(buf[index])) == 1)
			{
				if (mini_isspace(buf[index]) == 0)
				{
					tokencurrent = ft_substr(buf, index, 1);
					tokenarr = add_to_strarr(tokenarr, tokencurrent);
					free(tokencurrent);
				}
			}
			else
			{
				tokencurrent = ft_substr(buf, index, 2);
				tokenarr = add_to_strarr(tokenarr, tokencurrent);
				free(tokencurrent);
				index++;
			}
			start = index + 1;
		}
		index++;
	}	
	return (tokenarr);
}

// returns 1 if char is whitespace,
// else return 0
int mini_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

// checks if string is empty, only containing whitespaces
int	mini_isempty(char	*str)
{
	while (*str)
	{
		if (!mini_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}


// checks if its bash delimiter (|, spaces, \0, <, <<, >, >>)
// returns 0 if not delimiter, else number of spaces occpied by delimeter
int ft_isbashdelimiter(char *pt)
{
	if (ft_strncmp(pt, ">>", 2) == 0 || ft_strncmp(pt, "<<", 2) == 0)
		return (2);
	if (*pt == '|' || mini_isspace(*pt) || *pt == '<' || *pt == '>' || *pt == '$')
		return (1);
	return (0);
}

// checks if its a quote (', ")
// returns 0 if not quote, else 1
int ft_isquote(char pt)
{
	if (pt == '\'')
		return (1);
	if (pt == '"')
		return (2);
	return (0);
}


// Adds a string to the string array.
// If string array is NULL, it will be automatically initialised.
// String array always ends with a NULL pointer
// Returns the new string array, old one will be freed
char **add_to_strarr(char **oldstrarr, char *token)
{
	int oldsize;
	char *newentry;
	char **newtokenarr;

	if (mini_isempty(token))
		return (oldstrarr);
	oldsize = get_strarr_len(oldstrarr);
	newtokenarr = ft_calloc(sizeof(char *), oldsize + 2);
	newentry= ft_strdup(token);
	newtokenarr[oldsize] =  ft_strtrim(newentry, " \t\n\v\f\r");
	while (--oldsize >= 0)
		newtokenarr[oldsize] = oldstrarr[oldsize];
	free(oldstrarr);
	free(newentry);
	return (newtokenarr);
}

// Returns the length of the string array,
// NOT including the NULL pointer at the end.
int get_strarr_len(char **strarr)
{
	int index;

	index = 0;
	while (strarr && strarr[index])
		index++;
	return (index);
}

void print_str_arr(char **strarr)
{
	int index;

	index = 0;
	if (get_strarr_len(strarr) == 0)
		printf("STRING ARRAY IS EMPTY/NULL\n");
	else
	{
		while (strarr[index])
			printf("%s\n", strarr[index++]);
	}
}

int main(void)
{
	char *command = "ls -l|grep \"file\" > output.txt << input.txt>>output2.txt | echo $hello | echo \"$world\"" ;
	// char *command = "grep \"file>output";
	char **tokens;

	tokens = tokenise(command);
	print_str_arr(tokens);
	return (0);
}

// TESTS
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// // get_tokenarr_len(char **tokenarr);
// int main() {
//     char *tokenarr1[] = {"one", "two", "three", NULL};
//     char *tokenarr2[] = {"hello", "world", NULL};

//     // Test case 1
//     int len1 = get_tokenarr_len(tokenarr1);
//     printf("Length of tokenarr1: %d\n", len1);  // Expected output: 3
//     // Test case 2
//     int len2 = get_tokenarr_len(tokenarr2);
//     printf("Length of tokenarr2: %d\n", len2);  // Expected output: 2

//     return (0);
// }

// void add_to_tokenarr(char **oldtokenarr, char *token);

// int	main(void)
// {
// 	int		i;
// 	char	**tokenarr1;
// 	int		len1;
// 	int		len2;

// 	// Test case 1: Adding to an empty token array
// 	tokenarr1 = NULL;
// 	tokenarr1 = add_to_strarr(tokenarr1, "one");
// 	len1 = get_strarr_len(tokenarr1);
// 	printf("Length of tokenarr1 after adding 'one': %d\n", len1);
// 		// Expected output: 1
// 	// Displaying contents of tokenarr1 after modification
// 	printf("Contents of tokenarr1:\n");
// 	i = 0;
// 	while (tokenarr1[i])
// 		printf("%s\n", tokenarr1[i++]);

// 	// Test case 2: Adding to a non-empty token array
// 	tokenarr1 = add_to_strarr(tokenarr1, "two");
// 	len2 = get_strarr_len(tokenarr1);
// 	printf("Length of tokenarr1 after adding 'new': %d\n", len2);
// 		// Expected output: 2
// 	// Displaying contents of tokenarr1 after modification
// 	printf("Contents of tokenarr1:\n");

// 	i = 0;
// 	while (tokenarr1[i])
// 		printf("%s\n", tokenarr1[i++]);
// 	return (0);
// }

// tokenise function
