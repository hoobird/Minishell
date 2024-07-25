#include <string.h> // strlen()
#include <stdio.h> // printf()
#include <stdlib.h>
#include "lexer.h"
#include "../includes/minishell.h"

int main(void)
{
	const char	*content = 
//		"echo 'hello world' abc"; // expect: hello world abc (3 tokens)
		// "cat file1.txt > hello.txt haha >>hi'muah' | echo \"$SHELL@$USER\" '$LANGUAGE'"; //  something before and after '='
		"cat file1> fil'2'.t\"x\"t";

/*
handle '', replace with enclosed, less quotes
handle $ expansion, preserving space/lack of space, wrt to other tokens
handle "", expand $ within, preserviing space/lack of space wrt other tokens
*/
	Lexer		l = lexer_new(content, strlen(content));
	Token		t;
	size_t		t_count;
	
//	int i = 0;

	t_count = 0;

	t = lexer_next(&l);
	while (t.kind != TOKEN_END)
	{
// . means precision, and * means a placeholder for value in arg 
// so .* below, means print (int)t.text_len, chars, for the first %s
		printf("%.*s (%s)\n", (int) t.text_len, t.text, token_kind_name(t.kind));
		t = lexer_next(&l);
		t_count += 1;
	}
	printf("t_count: %zu\n", t_count);

/*	
	Token *myArr = ft_calloc(t_count + 1, sizeof(Token));
	
	l = lexer_new(content, strlen(content));
	
	
	int i = 0;
	while (i < t_count)
	{
		t = lexer_next(&l);
//		printf("token: %s\n", lexer_next(&l).text);
		myArr[i].kind = t.kind;
		myArr[i].text = ft_substr(t.text, 0, t.text_len);
		myArr[i].text_len = t.text_len;
		
		printf("kind %u\n", myArr[i].kind);	
		printf("text %s\n", myArr[i].text);
		printf("text_len %zu\n", myArr[i].text_len);			
		i++;	
	}
	
	while (i < t_count)
	{
		printf("%.*s (%s)\n", (int) myArr[i]->text_len, myArr[i]->text, token_kind_name(myArr[i]->kind)); 
		i++;	
	}
*/	
	
	
	
	return (0);
}

/*
youtube video until 56:00, covers handling invalid tokens
1:12, intro to literal tokens, RESTART here? 08 July 2024

# to do
## literal tokens
see video 1:12

## output to pass to next stage as array of token ptrs, Token *t_arr[]
why?

count number of tokens to store

## also needed?
void	free_token(Token *);
void	free_token_array(Token **);


*/
