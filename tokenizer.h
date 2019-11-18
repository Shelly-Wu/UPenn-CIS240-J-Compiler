#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "token.h"

//helper functions (implement these)
int read_word(token** tokens, char* line, int i);
int read_line(char* filename, token** tokens);
void start_asm_file(token *theToken, FILE* asm_file);
void store_const(token *theToken, FILE* asm_file);
void jump_to_function(token *theToken, FILE* asm_file);
void return_function(FILE* asm_file);
void new_fuction(FILE* asm_file);
void add_operation(FILE* asm_file);
void sub_operation(FILE* asm_file);
void mul_operation(FILE* asm_file);
void div_operation(FILE* asm_file);
void mod_operation(FILE* asm_file);
int lt_cmp(FILE* asm_file, int func_num);
int le_cmp(FILE* asm_file, int func_num);
int eq_cmp(FILE* asm_file, int func_num);
int ge_cmp(FILE* asm_file, int func_num);
int gt_cmp(FILE* asm_file, int func_num);
void and_logic(FILE* asm_file);
void not_logic(FILE* asm_file);
void or_logic(FILE* asm_file);
void xor_logic(FILE* asm_file);
void drop_operation(FILE* asm_file);
void dup_operation(FILE* asm_file);
void swap_operation(FILE* asm_file);
int min_operation(FILE* asm_file, int func_num);
void abs_operation(FILE* asm_file);
void avg_operation(FILE* asm_file);
void argN_operation(FILE* asm_file, int arg_num);
int pow_operation(FILE* asm_file, int func_num);
void if_statement(FILE* asm_file, int final_if, char* label_name);
void else_statement(FILE* asm_file, int final_if, char* label_name);
void endif_else_statement(FILE* asm_file, int final_if, char* label_name);
void endif_noelse_statement(FILE* asm_file, int final_if, char* label_name);

//theToken is the token to be populated
token* read_token (token *theToken, char* input);

//used for debugging
void print_token (token *theToken);


#endif
