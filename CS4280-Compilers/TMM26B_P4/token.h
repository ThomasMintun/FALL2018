#ifndef TOKEN_H
#define TOKEN_H
#include <string>

typedef enum tokenId{
	EOF_tk, ERROR_tk, IDENT_tk, NUM_tk, EQUAL_tk, //4
    COLON_tk, PLUS_tk, MINUS_tk, ASTERISK_tk, SLASH_tk, //9 
    AMP_tk, PERCENT_tk, PERIOD_tk, LESSTHAN_tk, GREATERTHAN_tk, //14
    LPARENTHESIS_tk, RPARENTHESIS_tk, COMMA_tk, LBRACE_tk, RBRACE_tk, //19
	SEMICOLON_tk, LBRACKET_tk, RBRACKET_tk, //22
	//Keywords
	BEGIN_tk, END_tk, ITER_tk, VOID_tk, VAR_tk, RETURN_tk, //28
	READ_tk, PRINT_tk, PROGRAM_tk, COND_tk, THEN_tk, LET_tk,//34
} tok_id;

//Token definition
typedef struct Token{
	tok_id tkIdentifier; //corresponds to array indecies of tok_id enum above
	std::string tkString; //max string length is 8 "identifier"
	int lineNo = 0; //lineNumber token is ons
	Token(){
		this->tkIdentifier = ERROR_tk;
		this->tkString = "";
		this->lineNo = 0;
	}
	Token(tok_id tkIdentifier, std::string tkString, int lineNo){
		this->tkIdentifier = tkIdentifier; 
		this->tkString = tkString;
		this->lineNo = lineNo;
	}
} token_t;
					
#endif