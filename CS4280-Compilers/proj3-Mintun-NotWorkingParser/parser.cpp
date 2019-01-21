#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include <fstream>
#include <iostream>

//put depth++ first line every function

std::ifstream inFile;
/* declare token_t and lineNo to be used in scanner calls within CFG functions */
token_t tk;
int lineNo = 1;

std::string tokenNames[35] = {
"End of File", "Error", "Identifier", "Number", "Equal Sign", "Colon",
"Plus Sign", "Minus Sign", "Asterisk", "Slash", "Ampersand",
"Percent Sign", "Percent Sign", "Less Than sign", "Greater Than sign", "Period", "Left Parenthesis", "Right Parenthesis",
"Left Brace", "Left Brace", "Semicolon", "Left Bracket", "Right Bracket",
"Begin Keyword", "End Keyword", "iter Keyword", "void", "var",
"return Keyword", "read keyword", "print Keyword", "program Keyword", "cond Keyword",
"then Keyword", "let Keyword"
};

Node *parser(){
    Node *root; //root node
    tk=scan(inFile, &lineNo); //passing address of lineNo
    std::cout << tokenNames[tk.tkIdentifier];
    root=PROGRAM();//root = what program returns
    //Checks if the End of is the last token. If yes then the parsing is correct.
    if(tk.tkIdentifier==EOF_tk)
    {
        std::cout << "Successful parsing." << std::endl;
    }
    else
    { //Else it will display an error and terminate the program.
        error(EOF_tk,tk);
    }
    //possible exit here
    return root; //This returns the root which points to the rest of the tree.. this could be empty
}

void error(tok_id expected , token_t got){
    std::cout<<"PARSING ERROR: The Expected Token was : "<<tokenNames[expected]<<" but got : "<<tokenNames[got.tkIdentifier]<<" on line number : "<<got.lineNo<<std::endl;
    std::cout<<"The parsing was terminated. "<<std::endl;
    exit(1);
}

//<PROGRAM>  -> void <VARS> <BLOCK>
Node *PROGRAM(){
    int depth = 0;
    Node *node = new Node("<program>", depth);
    //std::cout << tokenNames[tk.tkIdentifier];
    //program function can only go to two things. The vars() and block() functions which are non-terminals.
    if (tk.tkIdentifier == VOID_tk){
        tk = scan(inFile, &lineNo);//get the next token..."consume" the next token
        node->child_1 =VARS(depth);//call vars and set child to what vars returns
        node->child_2 = BLOCK(depth);//call block and set child to what block returns
    }
    else {
        error(VOID_tk, tk);
    }
    return node;//keep this outside or else will get compiler warning if inside if statement for exiting non void function
}

//<BLOCK> ->  Begin <VARS> <STATS> End
Node *BLOCK(int depth){
    depth++;
    if(tk.tkIdentifier == BEGIN_tk){
        Node *node = new Node("<block>", depth);
        tk=scan(inFile, &lineNo); //Get the next token
        node->child_1 = VARS(depth);
        node->child_2 = STATS(depth);
        if(tk.tkIdentifier == END_tk){
            tk=scan(inFile, &lineNo); //Consumes the End token.
            return node;
        }
        else{
           error(END_tk, tk);
        }
    }
    else{
       error(BEGIN_tk,tk);
    }
}

//<VARS> ->  empty | Var Identifier
Node *VARS(int depth){
    depth++;
    //if the token Id is a Keyword Var then it will consume the tk and call the mvar() function
    //it follows it automatically follows it in the BNF.
    if(tk.tkIdentifier == LET_tk){
        Node* node = new Node("<vars>", depth);
        tk = scan(inFile, &lineNo); //consumes the next token
        if(tk.tkIdentifier == IDENT_tk){
            node->token_1 = tk; //First save the token then consume it.
            tk=scan(inFile, &lineNo); //consumes the Identifiers.
            if (tk.tkIdentifier == EQUAL_tk){
                tk = scan(inFile, &lineNo);//get the next token
                if (tk.tkIdentifier == NUM_tk){
                    node->token_2 = tk;//store value in the tree
                    node->child_1 = VARS(depth);
                    return node;
                }else{
                    error(NUM_tk, tk);
                }
            }else{
                error(EQUAL_tk, tk);
            }
        }
        else{
            error(IDENT_tk, tk);
        }
    }
    else
        return nullptr;//return pointer that points to nothing... aka an EMPTY PRODUCTION
}

Node *EXPR(int depth){
    depth++;

    Node *node = new Node("<expr>", depth);
    node->child_1 = A(depth);//function call to A function
    if(tk.tkIdentifier == SLASH_tk){
        node->token_1 = tk;//store the operator
        tk=scan(inFile, &lineNo); //Consumes the plus.
        node->child_2 = EXPR(depth);
        return node;
    }
    else if(tk.tkIdentifier == ASTERISK_tk){
        node->token_1 = tk;//store the operator
        tk=scan(inFile, &lineNo); //Consumes the plus.
        node->child_2 = EXPR(depth);
        return node;
    }
    else{
       return node;
    }
}
//<A> -> M +A | M-A | M
Node *A(int depth){
    depth++;

    Node *node = new Node("<A>", depth);
    node->child_1 = A(depth);//function call to A function
    if(tk.tkIdentifier == PLUS_tk){
        node->token_1 = tk;//store the operator
        tk=scan(inFile, &lineNo); //Consumes the plus.
        node->child_2 = EXPR(depth);
        return node;
    }
    else if(tk.tkIdentifier == MINUS_tk){
        node->token_1 = tk;//store the operator
        tk=scan(inFile, &lineNo); //Consumes the plus.
        node->child_2 = EXPR(depth);
        return node;
    }
    else{
       return node;
    }
}

//<M> ->  -M | R
Node *M(int depth){
    depth++;

    Node *node = new Node("<M>", depth);
    if(tk.tkIdentifier == MINUS_tk){
        node->token_1 = tk;//store the operator
        tk=scan(inFile, &lineNo);
        node->child_2 = M(depth);
        return node;
    }
    else{
        node->child_2 = R(depth);
        return node;
    }
}

//<R> ->  [ <EXPR> ] | Identifier | Number
Node *R(int depth){
    depth++;
    Node *node = new Node("<R>", depth);
    //Checks if the first is a open bracket.
    if(tk.tkIdentifier == LPARENTHESIS_tk){
        tk=scan(inFile, &lineNo); //Consuming the Left Bracket.
        node->child_1 = EXPR(depth);
        if(tk.tkIdentifier == RPARENTHESIS_tk){
            tk=scan(inFile, &lineNo); //Consumes the Right Bracket.
            return node;
        }
        else{
            error(RPARENTHESIS_tk,tk);
        }
    }
    else if(tk.tkIdentifier==IDENT_tk){
        node->token_1 = tk; //First save the token before it consumes it.
        tk=scan(inFile, &lineNo); //consumes the Number or Integer token.
        return node;
    }
    else if(tk.tkIdentifier==NUM_tk){
        node->token_1 = tk; //First store the token before consuming it.
        tk=scan(inFile, &lineNo); //consumes the Identifier token.
        return node;
    }
    else{
        std::cout<<"Expect to get a Left Bracket or Identifier or Number but got : "<<tokenNames[tk.tkIdentifier]<<" on line number : "<<tk.lineNo<<std::endl;
        exit(1);
    }
}

//<STATS> ->  <STAT>  <MSTAT>
Node *STATS(int depth){
    depth++;
    Node *node = new Node("<STATS>", depth);
    //This non-terminal function will only have two options.
    node->child_1 = STAT(depth);
    node->child_2 = MSTAT(depth);
    return node; //Returns when both execute in order.
}

//<MSTAT> ->  empty |  <STAT>  <MSTAT>
Node *MSTAT(int depth){
    depth++;
    Node *node = new Node("<MSTATS>", depth);
    //This non-terminal function will only have two options.
    node->child_1 = STAT(depth);
    node->child_2 = MSTAT(depth);
    return node; //Returns when both execute in order.
    if (tk.tkIdentifier == READ_tk || tk.tkIdentifier == PRINT_tk || tk.tkIdentifier == COND_tk || tk.tkIdentifier == IDENT_tk || tk.tkIdentifier == BEGIN_tk || tk.tkIdentifier == ITER_tk){
        node->child_1 = STAT(depth);
        node->child_2 = MSTAT(depth);
        return node; //Returns when both execute in order.
    }
    else{
        return nullptr;//covers empty production
    }
}

//<STAT> ->  <IN> | <OUT> | <BLOCK> | <IFF> | <LOOP> | <ASSIGN>
Node *STAT(int depth){
    depth++;

    Node *node = new Node("<STAT>", depth);
    if(tk.tkIdentifier==READ_tk)
    {
        node->child_1 = IN(depth);
        return node;
    }
    else if(tk.tkIdentifier==PRINT_tk)
    {
        node->child_1 = OUT(depth);
        return node;
    }
    else if(tk.tkIdentifier==BEGIN_tk)
    {
        node->child_1 = BLOCK(depth);
        return node;
    }
    else if(tk.tkIdentifier==COND_tk)
    {
        node->child_1 = COND(depth);
        return node;
    }
    else if (tk.tkIdentifier==ITER_tk)
    {
        node->child_1 = LOOP(depth);
        return node;
    }
    else if(tk.tkIdentifier==IDENT_tk)
    {
        node->child_1 = ASSIGN(depth);
        return node;
    }
    else
    {
        std::cout<<"Expected to get token to be loop, check, begin, ouput, input or Identifier but got : "<<tokenNames[tk.tkIdentifier]<<" on line number : "<<tk.lineNo<<std::endl;
    }
}

//<IN> ->  read (Identifier):
Node *IN(int depth){
    depth++;
    if(tk.tkIdentifier==READ_tk){
        Node *node = new Node("<in>", depth);
        tk=scan(inFile, &lineNo); //Consumes the input keyword
        if(tk.tkIdentifier==LPARENTHESIS_tk){
            tk=scan(inFile, &lineNo);
            if(tk.tkIdentifier==IDENT_tk){
                node->token_1 = tk;//store token in the <IN> node
                tk=scan(inFile, &lineNo);
                if (tk.tkIdentifier == RPARENTHESIS_tk){
                    tk=scan(inFile, &lineNo);
                    if (tk.tkIdentifier == COLON_tk){
                        tk = scan(inFile, &lineNo);
                        return node;
                    }
                    else{
                        error(COLON_tk, tk);
                    }
                }
                else{
                    error(RPARENTHESIS_tk, tk);
                }
            }
            else
            {
                error(IDENT_tk,tk);
            }
        }
        else
        {
            error(LPARENTHESIS_tk,tk);
        }
    }
    else
    {
        error(READ_tk,tk);
    }
}

//<OUT> ->  Output <EXPR>  ;
Node *OUT(int depth)
{
    depth++;
    Node *node = new Node("<OUT>", depth);
    if(tk.tkIdentifier==PRINT_tk){
        tk=scan(inFile, &lineNo); //Consumes the input keyword
        if(tk.tkIdentifier==LPARENTHESIS_tk){
            tk=scan(inFile, &lineNo);
            node->child_1 = EXPR(depth);
                if (tk.tkIdentifier == RPARENTHESIS_tk){
                    tk=scan(inFile, &lineNo);
                    if (tk.tkIdentifier == COLON_tk){
                        tk = scan(inFile, &lineNo);
                        return node;
                    }
                    else{
                        error(COLON_tk, tk);
                    }
                }
                else{
                    error(RPARENTHESIS_tk, tk);
                }
        }
        else
        {
            error(LPARENTHESIS_tk,tk);
        }
    }
    else
    {
        error(PRINT_tk,tk);
    }
}

Node *COND(int depth){
    depth++;
    Node *node = new Node("<COND>", depth);
    //This starts the check and then goes to Left Bracket and then EXPR and Then RO and Then EXPR and then Right Bracket and Then STAT.
    //There is only one way that the tokens can go.
    if(tk.tkIdentifier==COND_tk)
    {
        tk=scan(inFile, &lineNo); //Consumes the Check Keyword
        if(tk.tkIdentifier==LPARENTHESIS_tk)
        {
            tk=scan(inFile, &lineNo); //Consumes the Left Bracket token.
            node->child_1 = EXPR(depth);
            node->child_2 = RO(depth);
            node->child_3 = EXPR(depth);
            if(tk.tkIdentifier==RPARENTHESIS_tk)
            {
                tk=scan(inFile, &lineNo); //Consumes the Right Bracket token.
                node->child_4 = STAT(depth);
                return node;
            }
            else {
                error(RPARENTHESIS_tk,tk);
            }
        }
        else {
            error(LPARENTHESIS_tk,tk);
        }
    }
    else{
        error(COND_tk, tk);
    }
}

//<LOOP>   ->  Loop [ <EXPR> <RO> <EXPR> ] <STAT>
Node *LOOP(int depth)
{
    depth++;
    Node *node = new Node("<LOOP>", depth);
    //This starts the check and then goes to Left Bracket and then EXPR and Then RO and Then EXPR and then Right Bracket and Then STAT.
    //There is only one way that the tokens can go.
    if(tk.tkIdentifier==ITER_tk)
    {
        tk=scan(inFile, &lineNo); //Consumes the Check Keyword
        if(tk.tkIdentifier==LPARENTHESIS_tk)
        {
            tk=scan(inFile, &lineNo); //Consumes the Left Bracket token.
            node->child_1 = EXPR(depth);
            node->child_2 = RO(depth);
            node->child_3 = EXPR(depth);
            if(tk.tkIdentifier==RPARENTHESIS_tk)
            {
                tk=scan(inFile, &lineNo); //Consumes the Right Bracket token.
                node->child_4 = STAT(depth);
                return node;
            }
            else {
                error(RPARENTHESIS_tk,tk);
            }
        }
        else {
            error(LPARENTHESIS_tk,tk);
        }
    }
    else{
        error(ITER_tk, tk);
    }
}

//<ASSIGN>   ->  Identifier : <EXPR>   ;
Node *ASSIGN(int depth)
{
    depth++;
    Node *node = new Node("<ASSIGN>", depth);
    if(tk.tkIdentifier==IDENT_tk)
    {
        node->token_1 = tk; //Save the token before consuming it.
        tk=scan(inFile, &lineNo); //Consumes the Identifier
        if(tk.tkIdentifier==EQUAL_tk)
        {
            tk=scan(inFile, &lineNo); //Consumes the Colon Identifier
            node->child_1 = EXPR(depth);
            if(tk.tkIdentifier==COLON_tk)
            {
                tk=scan(inFile, &lineNo); //Consumes the Semi-Colon Delimiter.
                return node;
            }
            else
            {
                error(COLON_tk, tk);
            }
        }
        else{
            error(EQUAL_tk,tk);
        }
    } else{
        error(IDENT_tk,tk);
    }
}

//<RO> ->  <= | >= | == | < | > | =
Node *RO(int depth)
{
    depth++;
    Node *node = new Node("<RO>", depth);
    if(tk.tkIdentifier==LESSTHAN_tk)
    {
        node->token_1 = tk; //store the token
        tk=scan(inFile, &lineNo); //Consumes the less than operator.
        if (tk.tkIdentifier == EQUAL_tk){
            node->token_2 = tk;
            tk=scan(inFile, &lineNo); //Consumes the less than operator.
        }
        return node;
    }
    else if(tk.tkIdentifier==GREATERTHAN_tk)
    {
        node->token_1 = tk; //store the token
        tk=scan(inFile, &lineNo); //Consumes the less than operator.
        if (tk.tkIdentifier == EQUAL_tk){
            node->token_2 = tk;
            tk=scan(inFile, &lineNo); //Consumes the less than operator.
        }
        return node;
    }
    else if(tk.tkIdentifier==EQUAL_tk)
    {
        node->token_1 = tk; //store the token
        tk=scan(inFile, &lineNo); //Consumes the less than operator.
        if (tk.tkIdentifier == EQUAL_tk){
            node->token_2 = tk;
            tk=scan(inFile, &lineNo); //Consumes the less than operator.
        }
        return node;
    }
    else
    {
        std::cout<<"The token that was received  was : "<<tokenNames[tk.tkIdentifier]<<" but expected to get < or <= or > or >+ or == or != on line number : "<<tk.lineNo<<std::endl;
        exit(1);
    }
}


void printTree(Node* node){
    if (node == nullptr){
        std::cout << "The tree is empty!";
        return;
    }
    else{
        std::string indents((node->depth * 2), ' ');//space character repeated depth * 2 times
        std::cout << indents;
        std::cout << node->name;
        std::cout << " ";
        std::cout << node->token_1.tkString << " ";
        std::cout << node->token_2.tkString << " ";
        std::cout << std::endl;
        if(node->child_1 != nullptr){
            printTree(node->child_1);
        }
        if(node->child_2 != nullptr){
            printTree(node->child_2);
        }
        if(node->child_3 != nullptr){
            printTree(node->child_3);
        }
        if(node->child_4 != nullptr){
            printTree(node->child_4);
        }        
    }
}