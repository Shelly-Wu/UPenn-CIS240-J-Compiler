#ifndef TOKEN_H
#define TOKEN_H

/*
 * token.h
 */

#include <stdio.h>

//assume tokens cannot be over this length
#define MAX_TOKEN_LENGTH 250

//type of token
typedef enum 
{ 
    DEFUN, IDENT, RETURN, 
    PLUS, MINUS, MUL, DIV, MOD, 
    AND, OR, NOT, 
    LT, LE, EQ, GE, GT,
    IF, ELSE, ENDIF, 
    DROP, DUP, SWAP, ARG, ABS, POW,
    LITERAL, XOR, MIN, AVG,
    BROKEN_TOKEN, EMPTY
} token_type;

typedef struct {
    token_type type;
    int literal_value;    // this field used to store the value of literal tokens
    int arg_no;           // this field used to store the index of argument literals argN
    char* str;
} token;

//returns a blank token
token* init_token();

//deletes a token
void delete_token(token* theToken);

#endif
