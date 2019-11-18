#include "hash.h"
#include "token.h"
#include "tokenizer.h"
#include <string.h>

int main(int argc, char *argv[]) 
{
    token *tokens[MAX_TOKEN_LENGTH * MAX_TOKEN_LENGTH];
    char *filename;
    FILE *asm_file;
    int token_num = 0;
    int offset = -1;
	  int func_num = 1;
    char *lastdot;
    int arg_num = 0;
    hash_struct* LOOPS = initTable(100);
    int if_max = -1;
    int final_if = -1;
    int else_seen[100];
    memset(else_seen, 0, 100*sizeof(int));
    if(argc < 2) {
        printf("error1: invalid input\n");
        done(LOOPS);
        return -1;
    }
    filename = argv[1];
    token_num = read_line(filename, tokens);

    char *new_name = malloc(strlen(filename + 2));
    char *name = malloc (strlen (filename) + 1);
    strcpy (name, filename);
    lastdot = strrchr (name, '.');
    *lastdot = '\0';
    sprintf(new_name, "%s.asm", name);
    asm_file = fopen(new_name, "w");
  
    for(int i = 0; i < token_num; i ++) {
        if (tokens[i]->type == DEFUN) {
            i ++;
            start_asm_file(tokens[i], asm_file);
        } else if (tokens[i]->type == LITERAL) {
            store_const(tokens[i], asm_file);
        } else if (i > 0 && tokens[i]->type == IDENT && tokens[i - 1]->type != DEFUN) {
            jump_to_function(tokens[i], asm_file);
        } else if (tokens[i]->type == RETURN) {
            return_function(asm_file);
        } else if (tokens[i]->type == PLUS) {
            add_operation(asm_file);
        } else if (tokens[i]->type == MINUS) {
            sub_operation(asm_file);
        } else if (tokens[i]->type == MUL) {
            mul_operation(asm_file);
        } else if (tokens[i]->type == DIV) {
            div_operation(asm_file);
        } else if (tokens[i]->type == MOD) {
            mod_operation(asm_file);
        } else if (tokens[i]->type == LT) {
					  func_num = lt_cmp(asm_file,func_num);
				} else if (tokens[i]->type == LE) {
					  func_num = le_cmp(asm_file,func_num);
				} else if (tokens[i]->type == EQ) {
					  func_num = eq_cmp(asm_file,func_num);
				} else if (tokens[i]->type == GE) {
					  func_num = ge_cmp(asm_file,func_num);
				} else if (tokens[i]->type == GT) {
					  func_num = gt_cmp(asm_file,func_num);
				} else if (tokens[i]->type == AND) {
            and_logic(asm_file);
        } else if (tokens[i]->type == OR) {
            or_logic(asm_file);
        } else if (tokens[i]->type == NOT) {
            not_logic(asm_file);
        } else if (tokens[i]->type == XOR) {
            xor_logic(asm_file);
        } else if (tokens[i]->type == DROP) {
            drop_operation(asm_file);
        } else if (tokens[i]->type == DUP) {
            dup_operation(asm_file);
        } else if (tokens[i]->type == SWAP) {
            swap_operation(asm_file);
        } else if (tokens[i]->type == MIN) {
            func_num = min_operation(asm_file, func_num);
        } else if (tokens[i]->type == ABS) {
            abs_operation(asm_file);
        } else if (tokens[i]->type == AVG) {
            avg_operation(asm_file);
        } else if (tokens[i]->type == ARG) {
            arg_num = tokens[i]->arg_no;
            argN_operation(asm_file, arg_num);
        } else if (tokens[i]->type == POW) {
            func_num = pow_operation(asm_file, func_num);
        } else if (tokens[i]->type == IF) {
            else_seen[if_max] = 0;
            if_max += 1;
            put(LOOPS, if_max, final_if);
            final_if = if_max;
            if_statement(asm_file, final_if, name);
        } else if(tokens[i]->type == ELSE) {
            else_seen[final_if] = 1;
            else_statement(asm_file, final_if, name);
        } else if(tokens[i]->type == ENDIF) {
            if(else_seen[final_if] == 0) {
                else_statement(asm_file, final_if, name);
            } 
            endif_noelse_statement(asm_file, final_if, name);
            final_if = get(LOOPS, final_if)->value;
        }
    }
    fclose(asm_file);
  
    for(int i = 0; i < token_num; i ++) {
        delete_token(tokens[i]);
    }
    return 0;
}
