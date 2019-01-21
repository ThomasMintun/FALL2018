#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "testScanner.h"
#include "token.h"
#include "scanner.h"


//Token name array to display on terminal feedback mapped to token enumeration order
std::string tokenNames[35] = {
"End of File", "Error", "Identifier", "Number", "Equal Sign", "Colon",
"Plus Sign", "Minus Sign", "Asterisk", "Slash", "Ampersand",
"Percent Sign", "Percent Sign", "Less Than sign", "Greater Than sign", "Period", "Left Parenthesis", "Right Parenthesis",
"Left Brace", "Left Brace", "Semicolon", "Left Bracket", "Right Bracket",
"Begin Keyword", "End Keyword", "iter Keyword", "void Keyword", "var Keyword",
"return Keyword", "read keyword", "print Keyword", "program Keyword", "cond Keyword",
"then Keyword", "let Keyword"
};

//testScanner function calls scanner while input file !eof iand reports returned token values
void testScanner(std::ifstream& infile){
	token_t temp;
	int lineNo = 1;

	do{
		temp = scan(infile, &lineNo);
		if(temp.tkIdentifier == ERROR_tk){
		std::cout << "Scanner Error: " << temp.tkString << " Not Allowed At Line: " << temp.lineNo;
			//std::cout << ("Exiting\n");
			//exit(EXIT_FAILURE);
		}
		else{
		std::cout << "Read " << temp.tkIdentifier << " token with value: " << temp.tkString << " at line: " << temp.lineNo << std::endl;
		std::cout << std::endl;
		}
	} while(infile);
}