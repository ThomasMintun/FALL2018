#include <string.h>
#include "semantics.h"
#include <iostream>
#include <fstream>
#include <string>

token_t stack[maxStackSize];//initialize array of tokens to act as stack
static int stackVars = 0;//total number of vars on the stack. Current stack size
static int scopeStart = 0;//used to track/assign scope starting point on stack
int i = 0;//loop counter var
static int tempNum = 0;//temp vars count
static int labNum = 0;//labels count
char* tempArr[maxStackSize];//array holding temp variable pointers

//DONE
void stackInit(){//initialize all stack location strings to prevent comparison to uninitialized memory location(seg fault)
    for(i = 0; i < maxStackSize; i++){
        stack[i].tkString = "";//initialize string name of token to be blank
    }
    return;
}

//DONE
//initialize temp variable array pointers to NULL so only used(non-NULL) pointer can be printed later
void tempArrInit(void){
    for(i = 0; i < maxStackSize ; i++){
        tempArr[i] =NULL;
    }
    return;
}

//DONE
void push(token_t stack_tk){//stack push
	if(stackVars == maxStackSize){//warn stack overflow
		std::cout << "Stack Is Full\n";
		exit(EXIT_FAILURE);
	}
	//check for variable scope existence before insertion
	for(i = scopeStart; i < stackVars; i++){
		if(stack[i].tkString == stack_tk.tkString){
			std::cout << stack_tk.tkString << " Already Declared In This Scope On Line: " << stack[i].lineNo;
			exit(EXIT_FAILURE);
		}
	}	
	stack[stackVars] = stack_tk;//Push token to the stack
	out_file << "\nPUSH\t" << stack_tk.tkString << std::endl; //stack_tk.tkString << std::endl;
	stackVars++;//Increment number of variables on stack to keep stack size current
}


//DONE
void pop(int scopeStart){//pop scope vars when called
	for(i = stackVars; i > scopeStart; i--){
		//decrement number of variables on stack to simulate removal and keep stack size current
		stack[i].tkString = nullptr;
		out_file << "POP\n";//produce POP asm
		stackVars--;
	}
}

//DONE
//Make sure variable doesn't exist in the local scope stack before pushing onto stack
int find(Token tk){//look through stack to see if it exists in stack.
	//search current scope for var string and return position
	for(i = stackVars; i > scopeStart - 1; i--){
		if(tk.tkString == stack[i].tkString){
			return stackVars - 1 - i;//return distance from top of the stack
		}
	}
	return -1;//not found
}

//DONE
int varExists(Token tk){//make sure variable exists in scope (or parent) before use
	//std::cout << "\nsemantics::varExists\n";
	//search current scope for var string
	for(i = stackVars - 1; i > - 1; i--){
		if(stack[i].tkString == tk.tkString){
			 return stackVars - 1 - i;//return distance from top of stack
		}
	}
	return -1;//not found
}

/*	Next functions return a c-string
	"T#" for new temp where # is the temp var count
	"L#" for new label where # is the label count
*/
char* newTemp(){
    char* tempVar = new char[4];//allocate storage
    sprintf(tempVar, "T%d",tempNum++); //create temporary variable name/number
    tempArr[tempNum - 1] = tempVar;
    return tempVar;
}

char* newLab(){
    char* label = new char[4];
    sprintf(label, "L%d",labNum++);
    return label;
}

void semanticCheck(Node* node, int count){
	if(node == nullptr){//check for empty tree
		return;
	}
	//std::cout << "\n new call to semanticCheck::: " << node->name << std::endl;

	if(node->name == "<PROGRAM>"){//process program node
		int numVars = 0;//start of program has no vars
		//process expected possible number of children if not null (<program> can have two child nodes)
		if(node->child_1 != nullptr)
			semanticCheck(node->child_1, numVars);
		if(node->child_2 != nullptr)
			semanticCheck(node->child_2, numVars);
		out_file << "STOP\n";
		//writing_to_global_file("\nP\t0");//"USE THIS VARIABLE IN DIVISION" --> Ramon

		//print all non-NULL(used) temp vars up to a maximum of MAXSTACKSIZE
		for(i = 0; i < maxStackSize; i++){
			if(tempArr[i] != NULL)
				//fprintf(outFile, "%s 0\n", tempArr[i]);
				//out_file << node->name;
				out_file << tempArr[i] << "\t0\n";
		}
	}

	else if(node->name == "<VARS>"){
		int found;//distance to top of the stack
		scopeStart = stackVars;
		found = find(node->token_1);
		if(found == -1 || found > count){//if not found in scope or defined globally...
			push(node->token_1);//push onto stack
			//std::cout << "<VARS> node token_1 = " << node->token_1.tkString << std::endl;
			count += 1;
		}
		else if(found < count){//error/exit if already defined within scope
			//std::cout << node->token_1.tkString << "Is Already Defined Within This Scope On Line: " << node->token_1.lineNo << std::endl;
			exit(EXIT_FAILURE);
		}
		//process expected possible number of children if not null (<vars> can have one child node)
		if(node->child_1 != nullptr)
			//std::cout << "\n" << node->child_1->name << " is the name of child1 node in <VARS>\n";
			semanticCheck(node->child_1, count);
	}

	//NEEDS WORK
	else if(node->name == "<BLOCK>"){//process block node which yields <vars> <block>
		int numVars = 0;//new block resets scope and number of variables in scope
		scopeStart = stackVars;//set scope = TOS
		//process expected possible number of children if not null (<block> can have two child nodes)
		if(node->child_1 != nullptr){
			semanticCheck(node->child_1, numVars);
		}
		if(node->child_2 != nullptr)
			semanticCheck(node->child_2, numVars);
		pop(scopeStart);//pop scope variables before exit
	}

	//
	else if(node->name == "<EXPR>"){
		//std::cout << node->child_1->name << "   made it to <EXPR>\n";//FOR SOME REASON ERROR TOKEN IS COMING UP HERE.
		//std::cout << "@#@##@#@#\n";
		//semanticCheck(node->child_1, count);

		if(node->token_1.tkString != ""){
			semanticCheck(node->child_2, count);//call 2nd child which is <EXPR>
			char* tempVar = newTemp();//create new temp variable
			out_file << "\nSTORE\t" << tempVar;
			semanticCheck(node->child_1, count);//call first child
			if(node->token_1.tkIdentifier == SLASH_tk){
				out_file << "\nDIV\t" << tempVar << std::endl;
			}
			else
				out_file << "\nMULT\t" << tempVar << std::endl;
		}
		else//<A> production
			semanticCheck(node->child_1, count);
	}

	else if(node->name == "<A>"){
		//std::cout << "ANode name is: " << node->child_1->name;

		if(node->token_1.tkString != ""){
			semanticCheck(node->child_2, count);//call 2nd child which is <EXPR>
			char* tempVar = newTemp();//create new temp variable
			out_file << "\nSTORE\t" << node->token_1.tkString;
			semanticCheck(node->child_1, count);//call first child
			if(node->token_1.tkIdentifier == PLUS_tk){
				out_file << "\nADD\t" << tempVar << std::endl;
			}
			else
				out_file << "\nSUB\t" << tempVar << std::endl;
		}
		else//<M> production
			semanticCheck(node->child_1, count);
	}

	//done
	else if(node->name == "<M>"){
		if(node->child_1->name == "<R>"){//if child = <R>
			//std::cout << "in node <M> about to call semanticCheck on child_1\n";
			semanticCheck(node->child_1, count);//call child
		}
		else {//<R>
			semanticCheck(node->child_1, count);//call child
			out_file << "\nMULT -1";//print "MULT -1"
		}
	}	

	else if(node->name == "<R>"){
		std::string tempVar = newTemp();
		//Check to make sure identifier was declared before use
		if(node->child_1 != nullptr){//if R has a child
			// bool exists = varExists(node->token_1);
			// if(!exists){
			// 	std::cout << "<R>::Variable " << node->token_1.tkString << " On Line: " << node->token_1.lineNo << " Was Not Declared In This Scope.\n";
			// 	exit(EXIT_FAILURE);
			// }
			semanticCheck(node->child_1, count);//call child
		}
		else{//no child
			out_file << "\nLOAD " << node->token_1.tkString;
		}
	}

	else if (node->name == "<IN>"){
		//std::cout << "\n<IN> node in semantics.cpp reached.\n" << node->name;
		//std::cout << " token_1 is: " << node->token_1.tkIdentifier << "and tkString is: " << node->token_1.tkString;
		// bool exists = varExists(node->token_1);
		// if(!exists){//took ! out from if(!exists)
		// 	std::cout << "<IN>::Variable " << node->token_1.tkString << " On Line: " << node->token_1.lineNo << " Was Not Declared In This Scope." << std::endl;
		// 	exit(EXIT_FAILURE);
		// }
		std::string tempVar = newTemp();
		out_file << "\nREAD " << tempVar;
		out_file << "\nLOAD " << tempVar;
		out_file << "\nSTACKW " << tempVar;	//????
	}

	else if (node->name == "<OUT>"){
		if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
		char* tempVar = newTemp();
		out_file << "\nSTORE " << tempVar << "\n";
		out_file << "\nWRITE " << tempVar << "\n";
		//std::cout << "tempVar:: " << tempVar;//for testing
		// bool exists = varExists(node->token_1);
		// if(!exists){//took ! out from if(!exists)
		// 	std::cout << "node->token_1.tkString.  " << node->token_1.tkString << " On Line: " << node->token_1.lineNo << " Was Not Declared In This Scope." << std::endl;
		// 	exit(EXIT_FAILURE);
		// }
	}

	//NEEDS WORK
	else if (node->name == "<IF>"){
		tok_id condition = node->child_2->token_1.tkIdentifier;//condition token used for comparison below
		if(node->child_3 != NULL)
			semanticCheck(node->child_3, count);//call child_3
			char* tempVar = newTemp();//create new temp
			out_file << "\nSTORE " << tempVar << "\n";
		if(node->child_1 != NULL)
			semanticCheck(node->child_1, count);//call child_1
			out_file << "\nSUB " << tempVar << "\n";
			char* label = newLab();
		//determine if > or >= exists
		if (condition == GREATERTHAN_tk){
			if (node->child_2->token_2.tkIdentifier == EQUAL_tk){
				out_file << "\nBRZNEG\t" << label;
			}
			else
				out_file << "\nBRZNEG\t" << label;
		}
		//determine if < or <= exists
		else if (condition == LESSTHAN_tk){
			if (node->child_2->token_2.tkIdentifier == EQUAL_tk){
				out_file << "\nBRPOS\t" << label;
			}
			else
				out_file << "\nBRPOS\t" << label;
		}
		else if (condition == EQUAL_tk){//determine if = or == exists
			if (node->child_2->token_2.tkIdentifier == EQUAL_tk){
				out_file << "\nBRPOS\t" << label;
				out_file << "\nBRNEG\t" << label;
			}
			else
				out_file << "\nBRPOS\t" << label;
				out_file << "\nBRNEG\t" << label;
		}
		else{//assume =! exists
			out_file << "BRZERO\t" << label;
		}
		if(node->child_4 != NULL)
			semanticCheck(node->child_4, count);//call child_4
			out_file << "\n" << label << " NOOP\n";
	}

	//NEEDS WORK
	else if (node->name == "<LOOP>"){
		tok_id condition = node->child_2->token_1.tkIdentifier;
		char *tempVar = newTemp();
		char *start = newLab();
		char *end = newLab();
		out_file << start << "\nNOOP\n";
		if(node->child_3 != NULL)
			semanticCheck(node->child_3, count);
		out_file << "\nSTORE " << tempVar << "\n";
		if(node->child_1 != NULL)
			semanticCheck(node->child_1, count);
		out_file << "\nSUB " << tempVar << "\n";
		if (condition == GREATERTHAN_tk){ // >
			out_file << "\nBRZPOS\t";
			if (node->child_2->token_2.tkIdentifier == EQUAL_tk){ // >=
				out_file << "\nBRZPOS=\t";
			}
		}
		else if (condition == LESSTHAN_tk){ // <
			out_file << "\nBRZNEG\t";
			if (node->child_2->token_2.tkIdentifier == EQUAL_tk){ // <=
				out_file << "\nBRZNEG\t";
			}
		}
		else if (condition == EQUAL_tk && node->child_2->token_2.tkIdentifier == EQUAL_tk){
			out_file << "\nBRNEG\t";
			if (node->child_2->token_2.tkIdentifier == EQUAL_tk){
				out_file << "\nEQUAL\t";
			}
		}
		else{
			out_file << "\nBRZERO\t";
		}
		if(node->child_4 != NULL)
			semanticCheck(node->child_4, count);
			out_file << "BR\t" << start << "\n";
			out_file << end << "\tNOOP" << "\n";
	}	

	else if(node->name == "<ASSIGN>"){
		push(node->token_1);
		//std::cout << "\nGot out of that push!\n";
		//std::cout << "Node name is: $$$" << node->name;
		//std::cout << " whos child_1 is " << node->child_1->name << "$$$\n";
		if(node->child_1->name == "<EXPR>")
			semanticCheck(node->child_1, count);
		bool exists = varExists(node->token_1);
		if(!exists){
			//std::cout << "<ASSIGN>::Variable " << node->token_1.tkString << " On Line: " << node->token_1.lineNo << " Was Not Declared In This Scope." << std::endl;
			exit(EXIT_FAILURE);
		}
		out_file << "STACKW " << tempNum << "\n";//tempVar needs to be changed WAS WRITE TO OUTFILE BUT CHANGED FOR TESTING
	}

	//Catch all to process productions with only non-terminals
	else{
		if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
		if(node->child_2 != nullptr)
			semanticCheck(node->child_2, count);
		if(node->child_3 != nullptr)
			semanticCheck(node->child_3, count);
		if(node->child_4 != nullptr)
			semanticCheck(node->child_4, count);
	}
	return;
}

// void writing_to_global_file(std::string any_var_you_want){
//     out_file << "GLOBAL\n";
//     //out_file << "Passed argument: " << any_var_you_want << "\n";
//     out_file << any_var_you_want << "\n";
// }
