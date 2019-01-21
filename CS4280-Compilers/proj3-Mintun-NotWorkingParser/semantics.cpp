#include <string.h>
#include "semantics.h"
#include <iostream>

//Initialize array of tokens to act as stack
token_t stack[maxStackSize];
/* Total number of vars on the stack. Current stack size */
static int stackVars = 0;
/* Used to track/assign scope starting point on stack */
static int scopeStart = 0;
/* Loop counter var */
int i = 0;

//Initialize all stack location strings to prevent comparison to uninitialized memory location(seg fault)
void stackInit(void){
	for(i = 0; i < maxStackSize + 1; i++){
		stack[i].tkString = "";//initialize string name of token to be blank
	}
	return;
}

//Stack push
void push(token_t stack_tk){
	//Warn stack overflow
	if(stackVars == maxStackSize){
		std::cout << "Stack Is Full\n";
		//exit(EXIT_FAILURE);
	}
	//Check for variable scope existence before insertion
	for(i = scopeStart; i < stackVars; i++){
		//if(strcmp(stack[i].tkString, stack_tk.tkString) == 0){
		if(stack[i].tkString == stack_tk.tkString){
			std::cout << stack_tk.tkString << " Already Declared In This Scope On Line: " << stack[i].lineNo;
			//exit(EXIT_FAILURE);
		}
	}	
	stack[stackVars] = stack_tk;//Push token to the stack
	stackVars++;//Increment number of variables on stack to keep stack size current
}

//Pop scope vars when called
void pop(int scopeStart){
	for(i = stackVars; i > scopeStart; i--){
		//Decrement number of variables on stack to simulate removal and keep stack size current
		stackVars--;
		stack[i].tkString = nullptr;
	}
}

//Make sure variable doesn't exist in the local scope stack before pushing onto stack
int find(Token tk){//look through stack to see if it exists in stack.
	//Search current scope for var string and return position
	for(i = stackVars; i > scopeStart - 1; i--){
		if(tk.tkString == stack[i].tkString){
			return stackVars - 1 - i;//return distance from top of the stack
		}
	}
	return -1;//Not found
}

bool varExists(Token tk){//Make sure variable exists in scope (or parent) before use
	//Search current scope for var string and return position
	for(i = stackVars - 1; i > - 1; i--){
		if(stack[i].tkString == tk.tkString){
			return true;//return distance from top of stack
		}
	}
	return false;//Not found
}

void semanticCheck(Node* node, int count){
	///empty tree
	if(node == nullptr){
		return;
	}

	//Process program node
	if(node->name == "<program>"){
		int numVars = 0;//Start of program has no vars
		/* Process expected possible number of children if not null (<program> can have two child nodes) */
		if(node->child_1 != nullptr)
			semanticCheck(node->child_1, numVars);
		if(node->child_2 != nullptr)
			semanticCheck(node->child_2, numVars);
	}
	
	//Process vars node
	else if(node->name == "<vars>"){
		int found;//distance to top of the stack
		scopeStart = stackVars;
		found = find(node->token_1);
		//If not found in scope or defined globally, push onto stack
		if(found == -1 || found > count){
			push(node->token_1);
			count += 1;
		}
		/* Error/exit if already defined within scope */
		else if(found < count){
			std::cout << node->token_1.tkString << "Is Already Defined Within This Scope On Line: " << node->token_1.lineNo << std::endl;
			exit(EXIT_FAILURE);
		}
		/* Process expected possible number of children if not null (<vars> can have one child node) */
		if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
	}
	
	//Process block node
	else if(node->name, "<block>"){
		int numVars = 0;//New block resets scope and number of variables in scope
		scopeStart = stackVars;//Set scope = TOS
		//Process expected possible number of children if not null (<block> can have two child nodes)
		if(node->child_1 != nullptr)
		semanticCheck(node->child_1, numVars);
		if(node->child_2 != nullptr)
		semanticCheck(node->child_2, numVars);
		pop(scopeStart);//Pop scope variables before exit
	}	

	else if(node->name == "<expr>"){
		/* Process as correct prediction using info from parse*/
		if(node->token_1.tkIdentifier == ASTERISK_tk || node->token_1.tkIdentifier == SLASH_tk){
			if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
			if(node->child_2 != nullptr)
			semanticCheck(node->child_2, count);	
		}
		else if(node->child_1 != nullptr){
			semanticCheck(node->child_1, count);
		}
	}

	else if(node->name == "<A>"){
		if(node->token_1.tkIdentifier == PLUS_tk || node->token_1.tkIdentifier == MINUS_tk){
			if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
			if(node->child_2 != nullptr)
			semanticCheck(node->child_2, count);	
		}
		else {
			if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
		}
	}

	/*else if(node->name == "<M>"){//if else here is probably not needed because same outcome in either production
		if(node->token_1.tkIdentifier == MINUS_tk){
			if(node->child_1 != nullptr)
				semanticCheck(node->child_1, count);
		}
		else{
			if(node->child_1 != nullptr)
				semanticCheck(node->child_1, count);
		}
	}*/

	else if(node->name == "<T>"){
		if(node->token_1.tkIdentifier == PLUS_tk){
			if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
			if(node->child_2 != nullptr)
			semanticCheck(node->child_2, count);	
		}
		if(node->token_1.tkIdentifier == MINUS_tk){
			if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
			if(node->child_2 != nullptr)
			semanticCheck(node->child_2, count);	
		}
		else{
			if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
		}
	}	
	else if(node->name == "<F>"){
		if(node->token_1.tkIdentifier == AMP_tk){
			if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
		}
		else{
			if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
		}
	}
	else if(node->name == "<R>"){
		//Check to make sure identifier was declared before use
		if(node->token_1.tkIdentifier == IDENT_tk){
			bool exists = varExists(node->token_1);
			if(!exists){
				std::cout << "Variable " << node->token_1.tkString << " On Line: " << node->token_1.lineNo << " Was Not Declared In This Scope.\n";
				exit(EXIT_FAILURE);
			}
		}
		else if(node->child_1 != nullptr){
			semanticCheck(node->child_1, count);
		}
	}

	else if (node->name == "<in>"){
		bool exists = varExists(node->token_1);
		if(!exists){
			std::cout << "Variable " << node->token_1.tkString << " On Line: " << node->token_1.lineNo << " Was Not Declared In This Scope.\n";
			exit(EXIT_FAILURE);
		}	
	}

/*	else if(node->name == "<out>"){
		if(node->child_1 != nullptr)
			semanticCheck(node->child_1, count);
	}

	else if(strcmp(node->name, "<if>") == 0){
		if(node->child_1 != nullptr)
		semanticCheck(node->child_1, count);
		if(node->child_3 != nullptr)
		semanticCheck(node->child_3, count);
		if(node->child_4 != nullptr)
		semanticCheck(node->child_4, count);
	}

	else if(strcmp(node->name, "<loop>") == 0){
		if(node->child_1 != nullptr)
		semanticCheck(node->child_1, count);
		if(node->child_3 != nullptr)
		semanticCheck(node->child_3, count);
		if(node->child_4 != nullptr)
		semanticCheck(node->child_4, count);
	}
*/
	else if(node->name == "<assign>"){
		bool exists = varExists(node->token_1);
		if(!exists){
			std::cout << "Variable " << node->token_1.tkString << " On Line: " << node->token_1.lineNo << " Was Not Declared In This Scope." << std::endl;
			exit(EXIT_FAILURE);
		}
		if(node->child_1 != nullptr)
		semanticCheck(node->child_1, count);
	}
	/* Catch all to process productions with only non-terminals */
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