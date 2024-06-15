// # include "minishell.h"
#include "../includes/minishell.h"

char	**tokenise(char *buf);
char	**add_to_strarr(char **oldtokenarr, char *token);
int		get_strarr_len(char **tokenarr);
int		ft_isspace(char c);

// for testing purpose
void	print_str_arr(char **strarr);

// Tokenise a string
// cat -e "hello" | ls --> {"cat", "-e", "hello", "|", "ls"}
char	**tokenise(char *buf)
{
	char	**tokenarr;
	char	*tokencurrent;
	int		index;
	int		start;

	start = 0;
	index = 0;
	while (ft_isspace(buf[index]))
		index++;
	start = index;
	while (buf[index])
	{
		if (ft_isspace(buf[index]) || buf[index] == '|' || buf[index + 1] == 0)
		{
			if (buf[index + 1] == 0)
				index++;
			tokencurrent = ft_substr(buf, start, index - start);
			add_to_strarr(tokenarr, tokencurrent);
			if (buf[index] == '|')
				add_to_strarr(tokenarr, "|");
			start = index + 1;
		}
		index++;
	}
	return (tokenarr);
}

// returns 1 if char is whitespace,
// else return 0
int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

// Adds a string to the string array.
// If string array is NULL, it will be automatically initialised.
// String array always ends with a NULL pointer
// Returns the new string array, old one will be freed
char	**add_to_strarr(char **oldstrarr, char *token)
{
	int		oldsize;
	char	**newtokenarr;

	if (!oldstrarr)
		oldstrarr = ft_calloc(sizeof(char *), 1);
	oldsize = get_strarr_len(oldstrarr);
	newtokenarr = ft_calloc(sizeof(char *), oldsize + 2);
	newtokenarr[oldsize] = ft_strdup(token);
	while (--oldsize >= 0)
		newtokenarr[oldsize] = oldstrarr[oldsize];
	free(oldstrarr);
	return (newtokenarr);
}

// Returns the length of the string array,
// NOT including the NULL pointer at the end.
int	get_strarr_len(char **strarr)
{
	int	index;

	index = 0;
	while (strarr && strarr[index])
		index++;
	return (index);
}

void	print_str_arr(char **strarr)
{
	int	index;

	index = 0;
	if (get_strarr_len(strarr) == 0)
		printf("STRING ARRAY IS EMPTY/NULL\n");
	else
	{
		while (strarr[index])
			printf("%s\n", strarr[index++]);
	}
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

int	main(void)
{
	char *command = "echo -n 'Hello'";
	char **tokens;

	tokens = tokenise(command);
	print_str_arr(tokens);
	return (0);
}