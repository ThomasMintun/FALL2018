/* Begin inclusion-prevention mechanism */
#ifndef PARSER_H
#define PARSER_H
#include "node.h"

extern std::ifstream inFile;
extern std::ofstream out_file;

/* parser auxilary function prototype */
Node *parser();

//prototypes for all the functions.
Node *parser();
Node *PROGRAM();
Node *BLOCK(int);
Node *VARS(int);
Node *EXPR(int);
Node *A(int);
Node *M(int);
Node *R(int);
Node *STATS(int);
Node *STAT(int);
Node *MSTAT(int);
Node *IN(int);
Node *OUT(int);
Node *IF(int);//CHANGED FROM *COND
Node *LOOP(int);
Node *ASSIGN(int);
Node *RO(int);
void error(tok_id, token_t);
void printTree(Node* node);

#endif /* PARSER_H */
