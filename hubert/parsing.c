#include "minishell.h"
#include "parsing.h"
#include "builtin.h"

// checks if its bash delimiter (|, spaces, \0, <, <<, >, >>)
// returns 0 if not delimiter, else number of spaces occpied by delimeter
int ft_isbashdelimiter(char *pt)
{
	if (ft_strncmp(pt, ">>", 2) == 0 || ft_strncmp(pt, "<<", 2) == 0)
		return (2);
	if (*pt == '|' || *pt == ' ' || *pt == '\t' || *pt == '<' || *pt == '>' || *pt == '$')
		return (1);
	return (0);
}

// return 1 if there is space, returns 0 if theres no space
int	check_if_space(char *str)
{
	if (*str == ' ' || *str == '\t')
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
void check_then_add_token(t_token **token, char *word, t_tokentype type, int spaces)
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
				*token = init_tokenlist(word, type, spaces);
			else
				add_token(*token, word, type, spaces);
			return ;
		}
		i++;
	}
	free(word); // free the word if its just made up of spaces
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
			process_quotes_handle_in_out_quote(&token, str, &quoted, &start, i);
		}
		i++;
	}
	process_quotes_add_last_word(&token, str, start, i, quoted);
	return (token);
}

// helper function for process_quotes:
// 		add the word before the quote and the quote to the token list
void	process_quotes_handle_in_out_quote(t_token **token, char *str, char *quoted, int *start, int i)
{
	if (*quoted == 0) // if not in quote
	{
		if (i - *start > 0) // if there is something before the quote
			check_then_add_token(token, ft_substr(str, *start, i - *start), WORD, check_if_space(&str[i - 1]));
		*quoted = str[i];
		*start = i;
	}
	else // if in quote
	{
		if (*quoted == '\'') 
			check_then_add_token(token, ft_substr(str, *start+1, i - *start-1), SQUOTE, check_if_space(&str[i+1]));
		else
			check_then_add_token(token, ft_substr(str, *start+1, i - *start-1), DQUOTE, check_if_space(&str[i+1]));
		*quoted = 0;
		*start = i + 1;
	}
}

// helper function for process_quotes:
// 		add the last word to the token list
void	process_quotes_add_last_word(t_token **token, char *str, int start, int i, int quoted)
{
	if (quoted == 0)
	{
		if (i - start > 0)
			check_then_add_token(token, ft_substr(str, start, i - start), WORD, check_if_space(&str[i - 1]));
	}
	else
	{
			check_then_add_token(token, ft_substr(str, start+1, i - start-1), ERROR_UNCLOSED_QUOTES, 0);
	}
}

// check if there is unclosed quotes
int		check_error_process_quotes(t_token *token)
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
					if (i - start > 0) // if there is something before the redirection
						check_then_add_token(&revisedtoken, ft_substr(token->string, start, i - start), WORD, check_if_space(&token->string[i - 1]));
					type = check_redirection_pipe_type(&(token->string[i]));
					if (type == RE_APPEND || type == RE_HEREDOC)
					{
						check_then_add_token(&revisedtoken, ft_substr(token->string, i, 2), type, 0); // no need to check space
						i += 2;
					}
					else
					{
						check_then_add_token(&revisedtoken, ft_substr(token->string, i, 1), type, 0); // no need to check space
						i++;
					}
					start = i;
				}
				else
					i++;
			}
			if (i - start > 0)
				check_then_add_token(&revisedtoken, ft_substr(token->string, start, i - start), WORD, token->postspace); // last token inherit the token's postspace
		}
		else
			check_then_add_token(&revisedtoken, ft_strdup(token->string), token->type, token->postspace);
		token = token->next;
	}
	return (revisedtoken);
}

// check if redirection not next to each other
// check if pipe is not next to pipe
int	check_error_redirection_pipe(t_token *token)
{
	t_token	*nexttoken;

	if (token != NULL && token-> type == PIPE)
	{
		printerror("Syntax error near unexpected token `|'\n");
		return (1);
	} 
	while (token != NULL)
	{
		if (token->type == PIPE)
		{
			nexttoken = token->next;
			if (nexttoken == NULL || nexttoken->type == PIPE)
			{
				printerror("Syntax error near unexpected token `|'\n");
				return (1);
			}
		}
		else if (token->type == RE_OUTPUT || token->type == RE_APPEND || token->type == RE_INPUT || token->type == RE_HEREDOC)
		{
			nexttoken = token->next;
			if (nexttoken == NULL || nexttoken->type == PIPE || nexttoken->type == RE_OUTPUT || nexttoken->type == RE_APPEND || nexttoken->type == RE_INPUT || nexttoken->type == RE_HEREDOC)
			{
				printerror("Syntax error near unexpected token `");
				if (nexttoken != NULL)
					ft_putstr_fd(nexttoken->string, 2);
				else
					ft_putstr_fd("newline", 2);
				ft_putstr_fd("'\n", 2);
				return (1);
			}
		}
		token = token->next;
	}
	return (0);
}

//join redirects with file to the right
t_token	*joinredirects(t_token *token)
{
	t_token	*newtoken;
	t_token	*temp;
	t_token	*nexttoken;
	char	*combined;
	char	*tmp;

	newtoken = NULL;
	while (token != NULL)
	{
		if (token->type == RE_OUTPUT || token->type == RE_APPEND || token->type == RE_INPUT || token->type == RE_HEREDOC)
		{
			nexttoken = token->next;
			if (nexttoken->type == WORD)
				tmp = ft_strtrim(nexttoken->string, " ");
			else
				tmp= ft_strdup(nexttoken->string);
			combined = ft_strjoin(token->string, tmp);
			free(tmp);
			check_then_add_token(&newtoken, combined, token->type, nexttoken->postspace);
			token = nexttoken;
		}
		else
		{
			check_then_add_token(&newtoken, ft_strdup(token->string), token->type, token->postspace);
		}
		token = token->next;
	}
	return (newtoken);
}

// expand shell vars
t_token	*handle_shellvars(char **envp, t_token *token)
{
	t_token	*newtoken;
	t_token	*temp;
	char	*expanded;

	newtoken = NULL;
	while (token != NULL)
	{
		if (token->type == WORD || token->type == DQUOTE)
		{
			expanded = expandshellvar(token->string, envp);
			check_then_add_token(&newtoken, expanded, token->type, token->postspace);
		}
		else
			check_then_add_token(&newtoken, ft_strdup(token->string), token->type, token->postspace);
		token = token->next;
	}
	return (newtoken);
}

// retokenise the WORD token after expansion
t_token	*retoken_word_after_expansion(t_token *etokens)
{
	t_token	*newtoken;
	char	**splitted;
	int		i;

	newtoken = NULL;
	while (etokens != NULL)
	{
		if (etokens->type == WORD)
		{
			splitted = ft_split(etokens->string, ' ');
			i = 0;
			while (splitted[i])
			{
				check_then_add_token(&newtoken, ft_strdup(splitted[i]), WORD, 1);
				free(splitted[i]);
				i++;
			}
			free(splitted);
			get_lasttoken(newtoken)->postspace = etokens->postspace;
		}
		else
			check_then_add_token(&newtoken, ft_strdup(etokens->string), etokens->type, etokens->postspace);
		etokens = etokens->next;
	}
	return (newtoken);
}

// merge together words that are stuck together
t_token	*merge_stucktogether_words(t_token *etokens)
{
	t_token	*newtoken;
	t_token	*temp;
	char	*combined;
	char 	*tmpcombined;

	newtoken = NULL;
	while (etokens != NULL)
	{	
		temp = etokens;
		if (etokens->type != PIPE)
		{
			combined = ft_strdup(etokens->string);
			while (etokens->next != NULL && etokens->postspace == 0 && (etokens->next->type == WORD || etokens->next->type == SQUOTE || etokens->next->type == DQUOTE))
			{
				tmpcombined = ft_strjoin(combined, etokens->next->string);
				free(combined);
				combined = tmpcombined;
				etokens = etokens->next;
			}
			check_then_add_token(&newtoken, combined, temp->type, temp->postspace);
		}
		else
		{
			check_then_add_token(&newtoken, ft_strdup(etokens->string), etokens->type, etokens->postspace);
		}
		etokens = etokens->next;
	}
	if (etokens != NULL)
		check_then_add_token(&newtoken, ft_strdup(etokens->string), etokens->type, etokens->postspace);
	return (newtoken);
}

// count number of pipes in token list
int	count_pipes(t_token *token)
{
	int		count;

	count = 0;
	while (token != NULL)
	{
		if (token->type == PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

// split by pipe
// returns a list of token list
t_token	**split_by_pipe(t_token *token)
{
	t_token	**newtokens;
	int		i;
	int		pipes;

	newtokens = ft_calloc(count_pipes(token) + 2, sizeof(t_token *));
	pipes = count_pipes(token);
	i = 0;
	while (token != NULL)
	{
		if (token->type == PIPE)
		{
			i++;
			token = token->next;
		}
		else
		{
			if (newtokens[i] == NULL)
				newtokens[i] = init_tokenlist(ft_strdup(token->string), token->type, token->postspace);
			else
				add_token(newtokens[i], ft_strdup(token->string), token->type, token->postspace);
			token = token->next;
		}
	}
	return (newtokens);
}

// label commands and args now
// 1. if first token is a WORD or SQUOTE or DQUOTE, then its a command
// 2. if first token is a RE_OUTPUT, RE_APPEND, RE_INPUT, RE_HEREDOC, then move down the list until you find a WORD, SQUOTE, DQUOTE, then its a command
// FINALLY, AFTER COMMAND IS APPOINTED, FOLLOWING WORD SQUOTE DQUOTE ARE ARGS
// DONT NEED TO LABEL REDIRECTIONS
void	label_commands_args(t_token **tokenlistlist)
{
	int		i;
	t_token	*tokens;
	int		commandfound;

	i = 0;
	while (tokenlistlist[i] != NULL)
	{
		commandfound = 0;
		tokens = tokenlistlist[i];
		while (tokens != NULL)
		{
			if (commandfound == 0 && (tokens->type == WORD || tokens->type == SQUOTE || tokens->type == DQUOTE))
			{
				tokens->type = COMMAND;
				commandfound = 1;
			}
			else
			{
				if (tokens->type == WORD || tokens->type == SQUOTE || tokens->type == DQUOTE)
					tokens->type = ARGS;
			}
			tokens = tokens->next;
		}
		i++;
	}	
}

// the main parsing function
t_token	**parse_input(char *str, char **envp)
{
	t_token	*tokens;
	t_token	*revisedtokens;
	t_token	**parse_output;

	// STEP 1 - handle quotes
	tokens = process_quotes(str);
	// Step 1a - Error out when unclosed quotes
	if (check_error_process_quotes(tokens))
	{
		free_tokenlist(&tokens);
		return (NULL);
	}
	// STEP 2 - SPLIT WORD BY SPACES
	revisedtokens = retoken_word_after_expansion(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 3  - handle special operators | > >> < <<
	revisedtokens = handle_redirection_pipe(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// Step 3a - check if redirection not next to each other
	// 			check if pipe is not next to pipe
	if (check_error_redirection_pipe(tokens))
	{
		free_tokenlist(&tokens);
		return (NULL);
	}
	// Step 4 - 9 
	parse_output = parse_input_helper(tokens, envp);
	return (parse_output);
}

// parse_input_helper 
// step 4 onwards
t_token	**parse_input_helper(t_token *tokens, char **envp)
{
	t_token	*revisedtokens;
	t_token	**parse_output;

	// STEP 4 - join redirects with file to the right
	revisedtokens = joinredirects(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 5 - handle shell vars
	revisedtokens = handle_shellvars(envp, tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 6 - MERGE TOGETHER WORDS THAT ARE STUCK TOGETHER
	revisedtokens = merge_stucktogether_words(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 8 - split command and args
	parse_output = split_by_pipe(tokens);
	free_tokenlist(&tokens);
	// STEP 9 - label commands and args
	label_commands_args(parse_output);
	return (parse_output);
}

/*
// cc parsing.c token_linkedlist.c printerror.c builtin_env.c expand_shell_var2.c ../Libft/libft.a -g
int main(int argc, char **argv, char **envp)
{
	t_token *tokens;
	t_token *revisedtokens;
	t_token **parse_output;
	char	*str;
	char	**envpc;

	envpc = envp_copy(envp);
	str = ft_calloc(100, sizeof(char));
	// ft_strlcpy(str, "echo 'hello'  $USER   123\"asd\" | cat >file2 >> file3", 100);
	// ft_strlcpy(str, "echo 'hello world'    file.txt | \"adsasd\"boss'a'", 100);
	// ft_strlcpy(str, "cat file1.txt > hello.txt haha >>hi'muah' | echo \"$SHELL$USER\" '$LANGUAGE'>", 100);
	// ft_strlcpy(str, "  echo $USER  number2  \"    $USER\"      '$SHELL'$USER \"$SHELL\" 'sd' $PWD", 100);
	// ft_strlcpy(str, "cat file1> fil'2'.t\"x\"t", 100);
	// ft_strlcpy(str, "echo 'file1'\"ads\"'asd'", 100);
	// ft_strlcpy(str, "echo test \"$SHELL$USER\" $PWD \"$USER $SHLVL\" hello ", 100);
	ft_strlcpy(str, "", 100);
	printf("Command: ^%s\n", str);

	// STEP 1 - handle quotes
	tokens = process_quotes(str);
	free(str);
	// Step 1a - Error out when unclosed quotes
	if (check_error_process_quotes(tokens))
	{
		envpc_free(&envpc);
		free_tokenlist(&tokens);
		return (1);
	}
	// STEP 2 - SPLIT WORD BY SPACES
	revisedtokens = retoken_word_after_expansion(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 3  - handle special operators | > >> < <<
	revisedtokens = handle_redirection_pipe(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// Step 3a - check if redirection not next to each other
	// 			check if pipe is not next to pipe
	if (check_error_redirection_pipe(tokens))
	{	
		envpc_free(&envpc);
		free_tokenlist(&tokens);
		return (1);
	}
	// STEP 4 - join redirects with file to the right
	revisedtokens = joinredirects(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 5 - handle shell vars
	revisedtokens = handle_shellvars(envpc, tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	print_tokenlist(tokens);
	// STEP 6 - MERGE TOGETHER WORDS THAT ARE STUCK TOGETHER
	revisedtokens = merge_stucktogether_words(tokens);
	free_tokenlist(&tokens);
	tokens = revisedtokens;
	// STEP 8 - split command and args
	parse_output = split_by_pipe(tokens);
	free_tokenlist(&tokens);
	// STEP 9 - label commands and args
	label_commands_args(parse_output);
	int i = 0;
	while (parse_output[i] != NULL)
	{
		printf("Command: %d\n", i);
		print_tokenlist(parse_output[i]);
		i++;
	}
	free_tokenlistlist(&parse_output);
	// print_tokenlist(revisedtokens);
	// free_tokenlist(&revisedtokens);
	envpc_free(&envpc);
	return (0);
}
*/

