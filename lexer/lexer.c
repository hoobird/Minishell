#include "lexer.h"

const char	*token_kind_name(Token_Kind kind)
{
	switch (kind)
	{
		case TOKEN_END:
			return "end of content";
		case TOKEN_INVALID: // string
			return "invalid token";
		case TOKEN_SYMBOL:
			return "symbol";
		case TOKEN_SYMBOL_PLUS:
			return "symbol plus ...";

		case TOKEN_HEREDOC:
			return "heredoc";
		case TOKEN_APPEND:
			return "append";

		case TOKEN_REDIR_IN:
			return "redirect to IN file";
		case TOKEN_REDIR_OUT:
			return "redirect to OUT file";
		case TOKEN_SINGLE_QUOTE:
			return "single quote";
		case TOKEN_SINGLE_QUOTE_PLUS:
			return "single quote plus ...";

		case TOKEN_DOUBLE_QUOTES:
			return "double quotes";
		case TOKEN_DASH:
			return "dash sign";
		case TOKEN_PIPE:
			return "pipe char";
		case TOKEN_DOLLAR:
			return "dollar sign";
		case TOKEN_DOLLAR_PLUS:
			return "dollar sign plus ...";
			
		case TOKEN_COMMENT:
			return "comment";
		default:
			return "unknown";
	}
	return NULL;
/*
// TO DO: have to swap to below:

	if (kind == TOKEN_END)
		return ("end of content");
	else if (kind == TOKEN_INVALID)
		return ("invalid token");
	else if (kind == TOKEN_PIPE)
		return ("pipe operator");
	else if (kind == TOKEN_SYMBOL)
		return ("symbol");
	else
		return ("unknown token type");
	return (NULL);
*/	
}

// to keep track of current char, to be lexed
Lexer	lexer_new(const char *content, size_t content_len)
{
	Lexer	l = {0}; // init. a type Lexer struct
	l.content = content;
	l.content_len = content_len;
	return (l);
}

void	lexer_trim_left(Lexer *l)
{
	while (l->cursor < l->content_len && isspace(l->content[l->cursor]))
	{
		char	x = l->content[l->cursor];
		l->cursor += 1;
		if (x == '\n')
		{
			l->line += 1;
			l->bol = l->cursor;
		}
	}
}

bool	is_symbol_start(char x)
{
	return (isalpha(x) || x == '_');
}

bool	is_symbol(char x)
{
	return (isalnum(x) || x == '_');
}

// not sure if this is needed?
bool	is_maybe(char x)
{
	return (x == '"' || x == '\'' || x == '$');
}

Token	lexer_next(Lexer *l)
{
	lexer_trim_left(l);

/*
TO DO? copy text_len chars into a malloc~ed space
*/
	Token token = {
		.text = &l->content[l->cursor],
	};
	
	if (l->cursor >= l->content_len)
		return token; // NOTE. token.kind "defaults" to TOKEN_END
/* 
handle tokens with MULTIPLE chars first, eg. append / heredoc		
*/
	if (l->content[l->cursor] == '/' && l->content[l->cursor + 1] == '/')
	{
		token.kind = TOKEN_COMMENT;
		while (l->cursor < l->content_len && l->content[l->cursor] != '\n')
		{
			l->cursor += 2;
			token.text_len += 2;
		}
		return token;	
	}	
	
	if (l->content[l->cursor] == '>' && l->content[l->cursor + 1] == '>')
	{
		token.kind = TOKEN_APPEND;
		l->cursor += 2;
		token.text_len = 2;
		return token;
	}
	
	if (l->content[l->cursor] == '<' && l->content[l->cursor + 1] == '<')
	{
		token.kind = TOKEN_HEREDOC;
		l->cursor += 2;
		token.text_len = 2;
		return token;
	}

// handle tokens with SINGLE chars, last	
	if (l->content[l->cursor] == '|')
	{
		token.kind = TOKEN_PIPE;
		l->cursor += 1;
		token.text_len = 1;
		return token;
	}

/*
handles THREE cases
if: 
$ + SYMBOL_START, and maybe SYMBOLs, and space, will be TOKEN_DOLLAR
$ + SYMBOL_START, and maybe SYMBOLs, and ?, will be TOKEN_DOLLAR_PLUS

else:
$ by itself, will be TOKEN_INVALID
*/
	if (l->content[l->cursor] == '$') 
	{
		token.kind = TOKEN_DOLLAR;
		l->cursor += 1; // consume $ char
		token.text_len += 1; // increment text_len for $ char
		if (l->cursor < l->content_len && is_symbol_start(l->content[l->cursor]))
		{
			l->cursor += 1; // consume SYMBOL_START char
			token.text_len += 1; // increment text_len for SYMBOL_START char
			while (l->cursor < l->content_len && is_symbol(l->content[l->cursor]))
			{
				l->cursor += 1;
				token.text_len += 1;
			}
			if (l->content[l->cursor] == ' ')
			{
				return token;
			}
			else // if currChar is NOT space, then rename currToken as *_PLUS
			{
				token.kind = TOKEN_DOLLAR_PLUS;
				return token;
			}
		}
		else // treat a single $ char as invalid token
		{
			token.kind = TOKEN_INVALID;
			return token;
		}
	}

/*


*/
	if (l->content[l->cursor] == '\'')
	{
		token.kind = TOKEN_SINGLE_QUOTE;
		l->cursor += 1; // consume the OPENING single quote char
		token.text_len += 1; // increment text_len for OPENING single quote char
		while (l->cursor < l->content_len && l->content[l->cursor] != '\'')
		{		
			l->cursor += 1;
			token.text_len += 1;
		}

		if (l->content[l->cursor] != '\'')
		{
			token.kind = TOKEN_INVALID;
			return token;
		}
// if reach here, means current char is single quote
		l->cursor += 1; // consume the CLOSING single quote char
		token.text_len += 1; // increment text_len for CLOSING single quote char
/* 
if next char is space, then return token as-is
else if next char is not space, then return token.kind as single_quote_plus
*/
		if (l->content[l->cursor] == ' ')
		{
			return token;
		}
		else
		{
			token.kind = TOKEN_SINGLE_QUOTE_PLUS;
			return token;
		}		
	}
	
	if (l->content[l->cursor] == '"')
	{
		token.kind = TOKEN_DOUBLE_QUOTES;
		l->cursor += 1;
		token.text_len = 1;
		return token;
	}
	
	if (l->content[l->cursor] == '-')
	{
		token.kind = TOKEN_DASH;
		l->cursor += 1;
		token.text_len = 1;
		return token;
	}			

// *** IMPT! *** heredoc/append must come before redirect in/out!	
	if (l->content[l->cursor] == '>')
	{
		token.kind = TOKEN_REDIR_OUT;
		l->cursor += 1;
		token.text_len = 1;
		return token;
	}
	
	if (l->content[l->cursor] == '<')
	{
		token.kind = TOKEN_REDIR_IN;
		l->cursor += 1;
		token.text_len = 1;
		return token;
	}

	if (is_symbol_start(l->content[l->cursor]))
	{
		token.kind = TOKEN_SYMBOL;
		while (l->cursor < l->content_len && is_symbol(l->content[l->cursor]))
		{
			l->cursor += 1;
			token.text_len += 1;
		}
/* 
if reach here, means currChar is not a symbol, ie. neither alnum, nor underscore
but... currChar can be single/double quote, or $

so if currChar is indeed single/double quote, or $
need to let the "next stage handler" know, so change token kind to SYMBOL_PLUS

eg. 
"   echo 'hello   world'xyz abc'defxyz'def   "
echo (symbol)
'hello   world' (single quote plus ...)
xyz (symbol)
abc (symbol plus ...)
'defxyz' (single quote plus ...)
def (symbol)

ie. next stage handler should resolve the above to:
echo (symbol)
hello   worldxyz (symbol) // note. 3 spaces, is retained between hello and world
abcdefxyzdef (symbol)

*/
		if (l->cursor < l->content_len && is_maybe(l->content[l->cursor]))
		{
			token.kind = TOKEN_SYMBOL_PLUS;
			return token;
		}
		else // means current char is NOT single/double quote, or $
			return token;
	}	

// if reach here, means current char is NOT any of the token types above
// but we still have to "consume" the char, and so make it an TOKEN_INVALID type
	l->cursor += 1;
	token.kind = TOKEN_INVALID;
	token.text_len = 1;
	return (token);
/*
lexer_next TO DO ???
implement '-' for option flag?
*/
}
