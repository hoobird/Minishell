#include "minishell.h"
#include "parsing.h"

// checks if its bash delimiter (|, spaces, \0, <, <<, >, >>)
// returns 0 if not delimiter, else number of spaces occpied by delimeter
int ft_isbashdelimiter(char *pt)
{
	if (ft_strncmp(pt, ">>", 2) == 0 || ft_strncmp(pt, "<<", 2) == 0)
		return (2);
	if (*pt == '|' || *pt == 32 || *pt == '\t' || *pt == '<' || *pt == '>' || *pt == '$')
		return (1);
	return (0);
}

// check for redirections and pipes
t_tokentype	check_redirection_pipe_type(char *s)
{
	if (ft_strncmp(s, ">>", 2) == 0)
		return (RE_APPEND);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (RE_HEREDOC);
	if (ft_strncmp(s, ">", 1) == 0)
		return (RE_OUTPUT);
	if (ft_strncmp(s, "<",1)== 0)
		return (RE_INPUT);
	if (ft_strncmp(s, "|", 1) == 0)
		return (PIPE);
	return (0);
}

// add token with check if its empty string or just made up of spaces
// also check if token list is empty
void check_then_add_token(t_token **token, char *word, t_tokentype type)
{
	int 		i;

	if (ft_strlen(word) == 0) // check if string is empty, cuz dont want to add empty string
	{
		free(word);
		return ;
	}
	i = 0;
	while (word[i])
	{
		if (word[i] != ' ' && word[i] != '\t')
		{
			if (*token == NULL)
				*token = init_tokenlist(word, type);
			else
				add_token(*token, word, type);
			return ;
		}
		i++;
	}	
}
	


// split the string into tokens normal, single_quote, double_quote
t_token	*process_quotes(char *str)
{
	// as we traverse the string, we will keep track of whether we are inside a quote or not
	char		quoted;
	int		i;
	int 	start;
	t_token	*token;

	quoted = 0;
	i = 0;
	start = 0;
	token = NULL;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (quoted == 0)
			{
				// add whats before the quote
				if (i - start > 0)
					check_then_add_token(&token, ft_substr(str, start, i - start), WORD);
				quoted = str[i];
				start = i;
			}
			else
			{
				if (str[i] == '\'')
					check_then_add_token(&token, ft_substr(str, start, i - start + 1), SQUOTE);
				else
					check_then_add_token(&token, ft_substr(str, start, i - start + 1), DQUOTE);
				quoted = 0;
				start = i + 1;
			}
		}
		i++;
	}
	if (i - start > 0)
	{
		if (quoted == 0)
			check_then_add_token(&token, ft_substr(str, start, i - start), WORD);
		else
			check_then_add_token(&token, ft_substr(str, start, i - start), ERROR_UNCLOSED_QUOTES);
	}
	return (token);
}

// handle redirections and pipe chracter
t_token	*handle_redirection_pipe(t_token *token)
{
	int		i;
	int		start;
	t_token	*revisedtoken;
	t_tokentype type;

	revisedtoken = NULL;
	while (token != NULL)
	{
		if (token->type == WORD)
		{
			i = 0;
			start = 0;
			while (token->string[i])
			{
				if (check_redirection_pipe_type(&(token->string[i])))
				{
					if (i - start > 0)
						check_then_add_token(&revisedtoken, ft_substr(token->string, start, i - start), WORD);
					type = check_redirection_pipe_type(&(token->string[i]));
					if (type == RE_APPEND || type == RE_HEREDOC)
					{
						check_then_add_token(&revisedtoken, ft_substr(token->string, i, 2), type);
						i += 2;
					}
					else
					{
						check_then_add_token(&revisedtoken, ft_substr(token->string, i, 1), type);
						i++;
					}
					start = i;
				}
				else
					i++;
			}
			if (i - start > 0)
				check_then_add_token(&revisedtoken, ft_substr(token->string, start, i - start), WORD);
		}
		else
			check_then_add_token(&revisedtoken, ft_strdup(token->string), token->type);
		token = token->next;
	}
	return (revisedtoken);
}
// cc parsing.c token_linkedlist.c printerror.c ../Libft/libft.a
int main()
{
	t_token *tokens;
	t_token *revisedtokens;
	char	*str;

	str = ft_calloc(100, sizeof(char));
	// ft_strlcpy(str, "echo 'hello world'>>!file.txt", 100);
	// ft_strlcpy(str, "echo 'hello world'    file.txt | \"adsasd\"boss'a'", 100);
	ft_strlcpy(str, "cat file1.txt > \"hello.txt\" >> 'hi' | wc -l", 100);
	// ft_strlcpy(str, "", 100);
	tokens = process_quotes(str);
	revisedtokens = handle_redirection_pipe(tokens);
	print_tokenlist(revisedtokens);
	free_tokenlist(&tokens);
	free_tokenlist(&revisedtokens);
	free(str);
	return (0);
}