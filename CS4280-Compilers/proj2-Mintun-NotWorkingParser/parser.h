/* Begin inclusion-prevention mechanism */
#ifndef PARSER_H
#define PARSER_H
#include "node.h"

extern std::ifstream inFile;

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
Node *COND(int);
Node *LOOP(int);
Node *ASSIGN(int);
Node *RO(int);
void error(tok_id, token_t);

#endif /* PARSER_H */
