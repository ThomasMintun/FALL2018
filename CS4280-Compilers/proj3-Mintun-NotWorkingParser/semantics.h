#ifndef SEMANTICS_H
#define SEMANTICS_H
//#define MAXSTACKSIZE 100 //Stack size limit per project spec
#include "token.h"
#include "node.h"

static const int maxStackSize = 100;

//Stack operations
void push(token_t stack_tk);
void pop(int scopeStart);
void stackInit(void);
//Variable/stack search operations
int find(token_t);
bool varExists(token_t);
//Main semantic checking function
void semanticCheck(Node *node, int count);

#endif