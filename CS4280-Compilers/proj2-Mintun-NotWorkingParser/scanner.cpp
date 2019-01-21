#include <iostream>
#include <stdio.h>//
#include <unistd.h>//
#include <fstream>
#include <string>
#include "scanner.h"
#include "token.h"
using namespace std;

int DFASTable [22][23] = {
//  L    D    =    <    >    :    +    -    *    /   %    .    (    )    ,    {    }    ;    [    ]    WS  EOF
 {  1,   2,   3,   4,   5,   6,   7,   8,   9,  10, 11,  12,  13,  14,  15,  16,  17,  18,  19,  20,   0,   -1,  -2}, // 0 these are state numbers
 {  1,   1, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}, // 1
 {501,   2, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501}, // 2
 {502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502}, // 3
 {503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503}, // 4 
 {504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504}, // 5
 {507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507}, // 6 :Token
 {508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508}, // 7 +Token
 {509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509}, // 8 -Token
 {510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510}, // 9 *Token
 {511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511}, // 10 /Token
 {512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512}, // 11 %Token
 {513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513}, // 12 .Token
 {514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514}, // 13 (Token
 {515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515}, // 14 )Token
 {516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516}, // 15 ,Token
 {517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517}, // 16 {Token
 {518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518}, // 17 }Token
 {519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519}, // 18 ;Token
 {520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520}, // 19 [Token
 {521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521}, // 20 ]Token
 {522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522}  // 21
};
 
char_m specialChars[] = {//{left value is ascii value, right value is column} ARRAY SUBSCRIPTS START AT 0!
	{61, 2}, {60, 3}, {62, 4}, {58, 5}, {43, 6},		// =, <, >, :, +
	{45, 7}, {42, 8}, {47, 9}, {38, 10}, {37, 11},		// -, *, /, %, .
	{46, 12}, {40, 13}, {41, 14}, {44, 15}, {123, 16},	// ., (, ), ,, {
	{125, 17}, {59, 18}, {91, 19}, {91, 20}, {93, 21}	// }, ;, [, ], 
};

state_m finalStateMap[] = {//one for every symbol; integer; EOF; error; identifier
  {500, IDENT_tk}, {501, NUM_tk}, {502, EQUAL_tk}, {503, LESSTHAN_tk}, 
  {504, GREATERTHAN_tk}, {507, COLON_tk}, {508, PLUS_tk}, {509, MINUS_tk}, {510, ASTERISK_tk}, 
  {511, SLASH_tk}, {512, PERCENT_tk} , {513, PERIOD_tk}, {514, LPARENTHESIS_tk},
  {515, RPARENTHESIS_tk}, {516, COMMA_tk}, {517, LBRACE_tk}, {518, RBRACE_tk}, {519, SEMICOLON_tk},
  {520, LBRACKET_tk}, {521, RBRACKET_tk}, {-1, EOF_tk}, {-2, ERROR_tk}
};


token_t keywordLookupTable[] = {
{BEGIN_tk, "begin", 0}, {END_tk, "end", 0}, {ITER_tk, "iter", 0},
{VOID_tk, "void", 0}, {VAR_tk, "var", 0}, {RETURN_tk, "return", 0},
{READ_tk, "read", 0}, {PRINT_tk, "print", 0}, {PROGRAM_tk, "program", 0},
{COND_tk, "cond", 0}, {THEN_tk, "then", 0}, {LET_tk, "let", 0}
};


//Character mapping function which uses specialChars array of charMap structs
//to match those values. Letters, numbers and EOF are matched on the fly.
int matchValid(int currentChar) {
	//try to match characters
	if (isalpha(currentChar)) {
		return 0;//column number
	}
	//try to match numbers
	else if (isdigit(currentChar)) {
		return 1;
	}
	//try to match WS
	else if (isspace(currentChar)) {
		return 20;
	}
	//try to match EOF
	else if (currentChar == EOF) {
		return 22;
	}
	else if(currentChar != EOF || !isspace(currentChar) || !isdigit(currentChar) || !isalpha(currentChar)){ 
		int l;
		//loop through special characters looking for a match
		for (l = 0; l < SPECIALCHARS; l++) {
			if(specialChars[l].input == currentChar){
				return specialChars[l].column;
			}
		}
	}
	return -2;
}

//Lookup function to map CurrentWord to tokens
token_t lookup(int state, std::string searchString) {
	token_t returnTk;
	int i;
	for (i = 0; i < TOKENS; i++) {
		if (finalStateMap[i].state == state) {
			returnTk.tkIdentifier = finalStateMap[i].tkEnum;
			returnTk.tkString = searchString;
			returnTk.lineNo = 999;
		}
	}
	//State indicates this an identifier. Use keyword lookup
	//table to ensure it is not a keyword. If it is, prepare
	//the identifier as a keyword and return it.
	if (state == 500) {
		//for (i = 0; i < KEYWORDS; i++) {
			//std::cout << "K: " << keywordLookupTable[i].tkString;
			//std::cout << "  " << "S: " << searchString << std::endl;
			if (searchString ==  "begin"){
				//nothing is matching keyword lookup table to searchString
				returnTk.tkIdentifier = BEGIN_tk;
				returnTk.tkString = "begin";
				//returnTk.lineNo = 999; //999 is a default/testing value
			}
			else if (searchString ==  "end"){
				//nothing is matching keyword lookup table to searchString
				returnTk.tkIdentifier = END_tk;
				returnTk.tkString = "end";
				//returnTk.lineNo = 999; //999 is a default/testing value
			}
			else if (searchString ==  "iter"){
				//nothing is matching keyword lookup table to searchString
				returnTk.tkIdentifier = ITER_tk;
				returnTk.tkString = "iter";
				//returnTk.lineNo = 999; //999 is a default/testing value
			}
			if (searchString ==  "void"){
				//nothing is matching keyword lookup table to searchString
				returnTk.tkIdentifier = VOID_tk;
				returnTk.tkString = "void";
				//returnTk.lineNo = 999; //999 is a default/testing value
			}
			/////continue....
	}
return returnTk;
}

//Scanner Function
token_t scan(std::ifstream& infile, int *lineNo) {
	token_t returnTk;
	//DFA state implementation
	int currentState = 0;
	int nextState = 0;

	//initialize return token values to defaults
	returnTk.tkIdentifier = ERROR_tk;
	returnTk.tkString = "Unchanged Token";
	returnTk.lineNo = 0;

	//holder for ascii value of currently read character
	char currentChar;

	//DFAColumn where currentChar is represented in DFA table
	int DFAColumn;
	//string holder for multiple character tokens
	std::string currentWord;
	//string to allow character to be appended easily to currentWord above
	std::string currentCharStr;

	//Continnue to get characters from the file and build token
	//while not a final state and not an error state
	while (currentState < 500 && currentState > -1) {
		infile.get(currentChar);
		if(infile.eof()){
			returnTk.tkIdentifier = EOF_tk;
			returnTk.tkString = "End of File\n";
			returnTk.lineNo = 0;
			return returnTk;
		}

		//Comments are skipped beginning with # until space encountered
		if (currentChar == '#') {
			while (!isspace(currentChar)) {
				infile.get(currentChar);
			}
		}

		//Not a comment so look at currentChar to
		//find applicable DFA table column
		DFAColumn = matchValid(currentChar);
  		currentCharStr += currentChar;
		//Set nextState
		nextState = DFASTable[currentState][DFAColumn];

		//Handle invalid characters at the start of a token
		if (DFAColumn == 21) {
			returnTk.tkIdentifier = ERROR_tk;
			returnTk.lineNo = *lineNo;
			currentWord = currentCharStr;
			std::cout << "\nScanner Error: Invalid Character: " << currentChar << ".\n";
			returnTk.tkString = currentWord;
			return returnTk;
		}
		//If next state is a final, EOF or an error
		if (nextState > 499 || nextState == -1) {
			//EOF
			if (nextState == -1) {
				returnTk.tkIdentifier = EOF_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = "EOF";
				return returnTk;
			}
			else if (nextState > 499) {
				returnTk = lookup(nextState, currentWord);//idToken or Keyword token
				returnTk.lineNo = *lineNo;
				infile.unget();//put character back
				return returnTk;
			}
			else {
				infile.unget();
				char temp; 
				infile.get(temp);//get character from file and put it in temp
  				currentCharStr += temp;
				currentWord += currentCharStr;
				std::cout <<"\nScanner Error: Line: " << *lineNo << ". Invalid Character: " << currentChar << ". Not A Valid.\n" << currentWord;
				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = currentWord;
				return returnTk;
			}
		}
		//Final state not yet reached. continue building token
		else{
			if(!isspace(currentChar)){
			currentWord = currentCharStr;
			if(currentWord.length() == 1 && isupper(currentChar)){
				std::cout << "Error can not start varibale names with upper char.";
				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = "Can NOT start with capital letter.";
				//return error token
			}
			}
			//Check to impose 8 character/digit length rule
			if (currentWord.length() > 8){
				std::cout << "Scanner Error: Identifier/Number Length Exception" << currentWord << ">8\n";
				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = "Length is too long.";
				return returnTk;
			}

			//Handle \n
			if (currentChar == '\n') {
				(*lineNo)++;
			}
			//State transition
			currentState = nextState;
		}
		
	}
	
	//Gross error failure
	returnTk.tkIdentifier = ERROR_tk;
	returnTk.lineNo = *lineNo;
	returnTk.tkString = "Scanner Error: Scanner Failure\n";
	return returnTk;
}
