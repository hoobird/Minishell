#include "minishell.h"

// initialise the token list (linked list) with first token
t_token	*init_tokenlist(char *word, t_tokentype type, int spacesafter)
{
	t_token	*token;

	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (token == NULL)
	{
		printerror("Error: failed to allocate memory for token\n");
		return (NULL);
	}
	token->string = word;
	token->type = type;
	token->next = NULL;
	token->postspace=spacesafter;
	return (token);
}

// get token list length
int	tokenlist_len(t_token *token)
{
	int	len;

	len = 0;
	while (token != NULL)
	{
		len++;
		token = token->next;
	}
	return (len);
}

// get token list list length
int	tokenlistlist_len(t_token **tokenlist)
{
	int	i;

	i = 0;
	while (tokenlist[i] != NULL)
		i++;
	return (i);
}

// get last token
t_token	*get_lasttoken(t_token *token)
{
	while (token->next != NULL)
		token = token->next;
	return (token);
}

// add token to the end of the token list
void	add_token(t_token *token, char *word, t_tokentype type, int spacesafter)
{
	t_token	*newtoken;

	newtoken = (t_token *)ft_calloc(1, sizeof(t_token));
	if (newtoken == NULL)
	{
		printerror("Error: failed to allocate memory for token\n");
		return ;
	}
	newtoken->string = word;
	newtoken->type = type;
	newtoken->next = NULL;
	newtoken->postspace=spacesafter;
	get_lasttoken(token)->next = newtoken;
}

// add 1 token after specified token
void	add_token_after(t_token *token, char *word, t_tokentype type, int spacesafter)
{
	t_token	*newtoken;

	newtoken = (t_token *)ft_calloc(1, sizeof(t_token));
	if (newtoken == NULL)
	{
		printerror("Error: failed to allocate memory for new token\n");
		return ;
	}
	newtoken->string = word;
	newtoken->type = type;
	newtoken->next = token->next;
	newtoken->postspace=spacesafter;
	token->next = newtoken;
}

// get token's index position
int	get_tokenindex(t_token *token, t_token *target)
{
	int	index;

	index = 0;
	while (token != target)
	{
		index++;
		token = token->next;
	}
	return (index);
}

// get token at specified index position
t_token	*get_token(t_token *token, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		token = token->next;
		i++;
	}
	return (token);
}

int	search_token(t_token *token, char *word)
{
	int	index;

	index = 0;
	while (token != NULL)
	{
		if (ft_strcmp(token->string, word) == 0)
			return (index);
		token = token->next;
		index++;
	}
	return (-1);
}

// remove specified token from token list using index position
void	remove_token(t_token **token, int index)
{
	t_token	*next;
	t_token	*delete;
	t_token	*prev;

	if (index == 0)
	{
		delete = *token;
		*token = (*token)->next;
		free(delete->string);
		free(delete);
		return ;
	}
	prev = get_token(*token, index - 1);
	delete = prev->next;
	next = delete->next;
	prev->next = next;
	free(delete->string);
	free(delete);
}

// free token list
void	free_tokenlist(t_token **token)
{
	t_token	*temp;

	while (*token != NULL)
	{
		temp = (*token)->next;
		free((*token)->string);
		free(*token);
		*token = temp;
	}
	*token = NULL;
}

// free list of token list
void	free_tokenlistlist(t_token ***tokenlist)
{
	int	i;

	i = 0;
	while ((*tokenlist)[i] != NULL)
	{
		free_tokenlist(&(*tokenlist)[i]);
		i++;
	}
	free(*tokenlist);
	*tokenlist = NULL;
}

char	*tokenstostring(t_token *token)
{
	char	*str;
	char	*temp;

	if (token == NULL)
		return (NULL);
	str = ft_strdup("");
	while (token != NULL)
	{
		temp = ft_strjoin(str, token->string);
		free(str);
		str = temp;
		token = token->next;
	}
	return (str);
}

// get token type name
char	*get_tokentype(t_tokentype type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == SQUOTE)
		return ("SQUOTE");
	else if (type == DQUOTE)
		return ("DQUOTE");
	else if (type == RE_OUTPUT)
		return ("RE_OUTPUT");
	else if (type == RE_APPEND)
		return ("RE_APPEND");
	else if (type == RE_INPUT)
		return ("RE_INPUT");
	else if (type == RE_HEREDOC)
		return ("RE_HEREDOC");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == COMMAND)
		return ("COMMAND");
	else if (type == ARGS)
		return ("ARGS");
	else if (type == ERROR_UNCLOSED_QUOTES)
		return ("ERROR_UNCLOSED_QUOTES");
	else
		return ("UNKNOWN");
}

// print token list
void	print_tokenlist(t_token *token)
{
	int	i;

	if (token == NULL)
	{
		printf("Token list is empty\n");
		return ;
	}
	i = 0;
	while (token != NULL)
	{
		printf("Token %d: ^%s^ [type: %s] [spacesafter: %d]\n", i, token->string, get_tokentype(token->type), token->postspace);
		token = token->next;
		i++;
	}
}

void	print_tokenlistlist(t_token **tokenlist)
{
	int i = 0;
	while (tokenlist[i] != NULL)
	{
		printf("Command: %d\n", i);
		print_tokenlist(tokenlist[i]);
		i++;
	}
	if (i == 0)
		printf("Token list list is empty\n");
}

// check if token list list is empty and frees it
int	check_tokenlistlist_empty_and_free(t_token **tokenlist)
{
	if (tokenlist == NULL || tokenlist[0] == NULL)
	{
		free_tokenlistlist(&tokenlist);
		return (1);
	}
	return (0);
}


/*
// cc token_linkedlist.c printerror.c ../Libft/libft.a 
int	main()
{
	t_token	*token;
	t_token *last;
	char	*word1;
	char	*word2;
	char	*word3;
	char	*word4;

	word1 = ft_strdup("cat");
	word2 = ft_strdup("file.txt");
	word3 = ft_strdup(">");
	word4 = ft_strdup("output.txt");

	token = init_tokenlist(word1, WORD, 1);
	add_token(token, word2, WORD, 1);
	add_token(token, word3, RE_OUTPUT, 1);
	add_token(token, word4, WORD, 1);
	
	print_tokenlist(token);
	printf("Token list length: %d\n", tokenlist_len(token));
	// print last token
	last = get_lasttoken(token);
	printf("Last token: %s\n", last->string);
	remove_token(&token, 0);
	print_tokenlist(token);
	free_tokenlist(&token);
	return (0);

}
*/
