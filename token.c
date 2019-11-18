#include "token.h"
#include <stdlib.h>

token* init_token() 
{
    //make sure you initialize char* in token. The length of str will be MAX_TOKEN_LENGTH
    token* my_token = malloc(sizeof(token));
    my_token->type = EMPTY;
    my_token->literal_value = 0;
    my_token->arg_no = 0;
    my_token->str = malloc(MAX_TOKEN_LENGTH*sizeof(char));
    return my_token;
}

void delete_token(token* theToken)
{
    //make sure you delete char* in token
    free(theToken->str);
    free(theToken);
}
