#ifndef LEXER_H
#define LEXER_H

#include <stddef.h> // size_t
#include <stdbool.h> // type bool
#include <ctype.h> // isalpha() etc.
#include <assert.h>

typedef enum
{
	TOKEN_END = 0,
	TOKEN_INVALID,
	TOKEN_SYMBOL,
	TOKEN_SYMBOL_PLUS,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_DOUBLE_QUOTES,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_SINGLE_QUOTE,
	TOKEN_SINGLE_QUOTE_PLUS,
	TOKEN_DASH,
	TOKEN_PIPE,
	TOKEN_DOLLAR,
	TOKEN_DOLLAR_PLUS,
	TOKEN_COMMENT, // demo only; in bash, '#' until eol, is treated as comment
} Token_Kind;

const char	*token_kind_name(Token_Kind kind);

typedef struct s_token
{
	Token_Kind	kind; // eg. eof, pipe, symbol etc.
	const char	*text; // token's content
	size_t		text_len; // len of token's content
	
} Token;

// keeps track of lexing progress
typedef struct
{
	const char	*content; // src, ie. what we are lexing
	size_t		content_len; 
	size_t		cursor; // index of curr char, 0 to len-1
	size_t		line; 
	size_t		bol; // index of the beginning of line; not sure if needed, if only handling 1 line of input?
} Lexer;

// for each readline outputted, create a new Lexer type var
Lexer	lexer_new(const char* content, size_t content_len);

// returns a token
Token	lexer_next(Lexer *l);

#endif // LEXER_H
