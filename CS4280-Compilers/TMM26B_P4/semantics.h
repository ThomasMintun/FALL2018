#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "token.h"
#include "node.h"
#include <fstream>

static const int maxStackSize = 100;

extern std::ofstream out_file;
void writing_to_global_file(std::string);

//Stack operations
void push(token_t stack_tk);
void pop(int scopeStart);
void stackInit();

//temp car and label opreations
void tempArrInit();
char *newTemp(void);
char *newLab(void);


//Variable/stack search operations
int find(token_t);
int varExists(token_t);
//Main semantic checking function
void semanticCheck(Node *node, int count);

#endif