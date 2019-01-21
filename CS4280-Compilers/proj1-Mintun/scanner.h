#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#define SPECIALCHARS 20 //const integers
#define TOKENS 25 //const integers
#define KEYWORDS 12//const integers CHANGED

typedef struct charMap{//look at using map
	int input;
	int column;
}char_m;

typedef struct stateMap{
	int state;
	tok_id tkEnum;
}state_m;

token_t scan(std::ifstream& infile, int *lineNo);//scanner function
token_t lookup(int state, char *searchString);//token keyword mapping function
int matchValid(int currentChar);//char mapping function

#endif