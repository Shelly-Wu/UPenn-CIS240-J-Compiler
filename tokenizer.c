#include "tokenizer.h"
#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//theToken is the token to be populated
token* read_token (token *theToken, char* input) {
    theToken = init_token();
    strcpy(theToken->str,input);
    if(!strcmp(input,"defun")) {
        theToken->type = DEFUN;
    } else if(!strcmp(input,"return")) {
        theToken->type = RETURN;
    } else if(!strcmp(input,"+")) {
        theToken->type = PLUS;
    } else if(!strcmp(input,"-")) {
        theToken->type = MINUS;
    } else if(!strcmp(input,"*")) {
        theToken->type = MUL;
    } else if(!strcmp(input,"/")) {
        theToken->type = DIV;
    } else if(!strcmp(input,"%")) {
        theToken->type = MOD;
    } else if(!strcmp(input,"and")) {
        theToken->type = AND;
    } else if(!strcmp(input,"or")) {
        theToken->type = OR;
    } else if(!strcmp(input,"not")) {
        theToken->type = NOT;
    } else if(!strcmp(input,"xor")) {
        theToken->type = XOR;
    } else if(!strcmp(input,"lt")) {
        theToken->type = LT;
    } else if(!strcmp(input,"le")) {
        theToken->type = LE;
    } else if(!strcmp(input,"eq")) {
        theToken->type = EQ;
    } else if(!strcmp(input,"ge")) {
        theToken->type = GE;
    } else if(!strcmp(input,"gt")) {
        theToken->type = GT;
    } else if(!strcmp(input,"if")) {
        theToken->type = IF;
    } else if(!strcmp(input,"else")) {
        theToken->type = ELSE;
    } else if(!strcmp(input,"endif")) {
        theToken->type = ENDIF;
    } else if(!strcmp(input,"drop")) {
        theToken->type = DROP;
    } else if(!strcmp(input,"dup")) {
        theToken->type = DUP;
    } else if(!strcmp(input,"swap")) {
        theToken->type = SWAP;
    } else if(!strncmp(input,"arg", 3)) {
        theToken->type = ARG;
        theToken->arg_no = atoi(input+3);
    } else if(!strcmp(input,"abs")) {
        theToken->type = ABS;
    } else if(!strcmp(input,"^")) {
        theToken->type = POW;
    } else if (atoi(input)) {
        theToken->type = LITERAL;
        signed short int literal = atoi(input);
        theToken->literal_value = literal;
    } else if (!strncmp(input,"0x",2)) {
        if(atoi(input + 2)) {
            theToken->type = LITERAL;
            signed short int literal = strtol(input+2, NULL,16);
            theToken->literal_value = strtol(input+2, NULL,16);
        }
    } else if (!strncmp(input,"-",1)) {
        if(atoi(input + 1)) {
            theToken->type = LITERAL;
            signed short int literal = 0 - atoi(input);
            theToken->literal_value = literal;
        }
    } else if(!strcmp(input,"min")) {
        theToken->type = MIN;
    } else if(!strcmp(input,"avg")) {
        theToken->type = AVG;
    } else{
        if(!strcmp(input,"0")) {
            theToken->type = LITERAL;
            signed short int literal = 0;
        } else {
            int k = 0;
            for(k = 0; k < strlen(input); k ++) {
                if((input[k] >= 'a' && input[k] <= 'z')|| (input[k] >= 'A' && input[k] <= 'Z')\
                 || (input[k] >= '0' && input[k] <= '9')  || input[k] == '_') {
                    continue;
                } else {
                    break;
                }
            }
            if(k >= strlen(input)) {
                theToken->type = IDENT;
            } else {
                theToken->type = BROKEN_TOKEN;
            }
        }
    }
    return theToken;
}

int read_word(token** tokens, char* line, int i){
    char* word;
    word = strtok(line, " \t");
    while (word != NULL) {
        tokens[i] = read_token(tokens[i], word);
        word = strtok(NULL, " \t");
        i ++ ;
    }
    return i;
}

int read_line(char* filename, token** tokens) {
    FILE* input_file = fopen(filename, "r");
    char line[MAX_TOKEN_LENGTH];
    int i = 0;
    int j = 0;
    if(input_file == NULL) {
        perror("Error opening file");
        return -1;
    }
    while (fgets(line, MAX_TOKEN_LENGTH, input_file) != NULL) {
        for (j = 0 ; j < strlen(line); j ++) {
            if (line[j] == ';') {
                line[j] = '\0';
             }
        }
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        i = read_word(tokens, line, i);
    }
    fclose(input_file);
    return i;
}


//used for debugging
void print_token (token *theToken) {
    printf("type is %d ", theToken->type);
    if(theToken->literal_value) {
        printf("literal_value is %d ", theToken->literal_value);
    }
    if(theToken->arg_no) {
        printf("arg_no is %d ", theToken->arg_no);
    }
    printf("str is %s\n", theToken->str);
}

void start_asm_file(token *theToken, FILE* asm_file) {
    fputs(".CODE\n", asm_file);
    fputs(".FALIGN\n", asm_file);
    fprintf(asm_file, "%s\n", theToken->str);
    fputs("  ;;prologue\n", asm_file);
    fputs("  STR R7, R6, #-2\n", asm_file);
    fputs("  STR R5, R6, #-3\n", asm_file);
    fputs("  ADD R6, R6, #-3\n", asm_file);
    fputs("  ADD R5, R6, #0\n\n", asm_file);
}
  
void store_const(token *theToken, FILE* asm_file) {
	  int high_const = (theToken->literal_value & 65280)/256 ;
	  int low_const = theToken->literal_value & 255;
    fprintf(asm_file, "  CONST R7, x%x\n", low_const);
  	fprintf(asm_file, "  HICONST R7, x%x\n", high_const);
    fputs("  ADD R6, R6, #-1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void jump_to_function(token *theToken, FILE* asm_file) {
    fprintf(asm_file, "  JSR %s\n\n", theToken->str);
}


void new_fuction(FILE* asm_file) {
	  fputs(".CODE\n", asm_file);
    fputs(".FALIGN\n", asm_file);
    fputs("  ;;prologue\n", asm_file);
}

void add_operation(FILE* asm_file) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  ADD R7, R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void sub_operation(FILE* asm_file) {
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  SUB R7, R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void mul_operation(FILE* asm_file) {
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  MUL R7, R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void div_operation(FILE* asm_file) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  DIV R7, R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void mod_operation(FILE* asm_file) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  MOD R7, R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}


int lt_cmp(FILE* asm_file, int func_num) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
	  fputs("  LDR R0, R6, #1\n", asm_file);
	  fputs("  CMP R7, R0\n", asm_file);
	  fprintf(asm_file, "  BRn SET_TRUE_%d\n", func_num);
	  fprintf(asm_file, "  BRzp SET_FALSE_%d\n", func_num);
	
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_TRUE_%d\n", func_num);
	  fputs("    CONST R7, #1\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_FALSE_%d\n", func_num);
	  fputs("    CONST R7, #0\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	
	  
	  fprintf(asm_file, "  CONTINUE_%d\n", func_num);
    fputs("  ADD R6, R6, #1\n", asm_file);
	  fputs("  STR R7, R6, #0\n\n", asm_file);
	  func_num ++ ;
	  return func_num;
}

int le_cmp(FILE* asm_file, int func_num) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
	  fputs("  LDR R0, R6, #1\n", asm_file);
	  fputs("  CMP R7, R0\n", asm_file);
	  fprintf(asm_file, "  BRnz SET_TRUE_%d\n", func_num);
	  fprintf(asm_file, "  BRp SET_FALSE_%d\n", func_num);

	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_TRUE_%d\n", func_num);
	  fputs("    CONST R7, #1\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_FALSE_%d\n", func_num);
	  fputs("    CONST R7, #0\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	
	  
	  fprintf(asm_file, "  CONTINUE_%d\n", func_num);
    fputs("  ADD R6, R6, #1\n", asm_file);
	  fputs("  STR R7, R6, #0\n\n", asm_file);
	  func_num ++ ;
	  return func_num;
}

int eq_cmp(FILE* asm_file, int func_num) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
	  fputs("  LDR R0, R6, #1\n", asm_file);
	  fputs("  CMP R7, R0\n", asm_file);
	  fprintf(asm_file, "  BRz SET_TRUE_%d\n", func_num);
	  fprintf(asm_file, "  BRnp SET_FALSE_%d\n", func_num);
	
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_TRUE_%d\n", func_num);
	  fputs("    CONST R7, #1\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_FALSE_%d\n", func_num);
	  fputs("    CONST R7, #0\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	
	  fprintf(asm_file, "  CONTINUE_%d\n", func_num);
    fputs("  ADD R6, R6, #1\n", asm_file);
	  fputs("  STR R7, R6, #0\n\n", asm_file);
	  func_num ++ ;
	  return func_num;
}

int ge_cmp(FILE* asm_file, int func_num) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
	  fputs("  LDR R0, R6, #1\n", asm_file);
	  fputs("  CMP R7, R0\n", asm_file);
	  fprintf(asm_file, "  BRzp SET_TRUE_%d\n", func_num);
	  fprintf(asm_file, "  BRn SET_FALSE_%d\n", func_num);

	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_TRUE_%d\n", func_num);
	  fputs("    CONST R7, #1\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_FALSE_%d\n", func_num);
	  fputs("    CONST R7, #0\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	  
	  fprintf(asm_file, "  CONTINUE_%d\n", func_num);
    fputs("  ADD R6, R6, #1\n", asm_file);
	  fputs("  STR R7, R6, #0\n\n", asm_file);
	  func_num ++ ;
	  return func_num;
}

int gt_cmp(FILE* asm_file, int func_num) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
	  fputs("  LDR R0, R6, #1\n", asm_file);
	  fputs("  CMP R7, R0\n", asm_file);
	  fprintf(asm_file, "  BRp SET_TRUE_%d\n", func_num);
	  fprintf(asm_file, "  BRnz SET_FALSE_%d\n", func_num);

	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_TRUE_%d\n", func_num);
	  fputs("    CONST R7, #1\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  SET_FALSE_%d\n", func_num);
	  fputs("    CONST R7, #0\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);

	  fprintf(asm_file, "  CONTINUE_%d\n", func_num);
    fputs("  ADD R6, R6, #1\n", asm_file);
	  fputs("  STR R7, R6, #0\n\n", asm_file);
	  func_num ++ ;
	  return func_num;
}

void and_logic(FILE* asm_file) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  AND R7, R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void not_logic(FILE* asm_file) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  NOT R7, R7\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void or_logic(FILE* asm_file) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  OR R7, R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void xor_logic(FILE* asm_file) {
	  fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  XOR R7, R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void drop_operation(FILE* asm_file) {
    fputs("  ADD R6, R6, #1\n", asm_file);
}

void dup_operation(FILE* asm_file) {
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  ADD R6, R6, #-1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void swap_operation(FILE* asm_file) {
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #1\n", asm_file);
    fputs("  STR R0, R6, #0\n", asm_file);
}

int min_operation(FILE* asm_file, int func_num) {
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  CMP R7, R0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fprintf(asm_file, "  BRn KEEP_PRE_%d\n", func_num); //should keep R7
	  fprintf(asm_file, "  BRzp KEEP_POST_%d\n", func_num); //should keep R0
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  KEEP_PRE_%d\n", func_num);
    fputs("  STR R7, R6, #0\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	  fputs("  .FALIGN\n", asm_file);
	  fprintf(asm_file, "  KEEP_POST_%d\n", func_num);
    fputs("  STR R0, R6, #0\n", asm_file);
	  fprintf(asm_file, "  JMP CONTINUE_%d\n", func_num);
	  fprintf(asm_file, "  CONTINUE_%d\n", func_num);
    func_num ++;
    return func_num;
}

void abs_operation(FILE* asm_file) {
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  CONST R0, #0\n", asm_file);
    fputs("  SUB R7, R0, R7\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void avg_operation(FILE* asm_file) {
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  LDR R0, R6, #1\n", asm_file);
    fputs("  LDR R1, R6, #2\n", asm_file);
    fputs("  CONST R2, #3\n", asm_file);
    fputs("  ADD R7, R7, R0\n", asm_file);
    fputs("  ADD R7, R7, R1\n", asm_file);
    fputs("  DIV R7, R7, R2\n", asm_file);
    fputs("  ADD R6, R6, #2\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

int pow_operation(FILE* asm_file, int func_num) {
    fputs("  CONST R7, #1\n", asm_file);
    fputs("  STR R7, R6, #-1\n", asm_file);
    fputs("  LDR R1, R6, #0\n", asm_file);
    fprintf(asm_file, "  JMP POW_%d\n", func_num);
    fprintf(asm_file, "  POWER_%d\n", func_num);
    fputs("  LDR R7, R6, #-1\n", asm_file);
    fputs("  MUL R7, R7, R1\n", asm_file);
    fputs("  STR R7, R6, #-1\n", asm_file);
    fputs("  LDR R7, R6, #1\n", asm_file);
    fputs("  ADD R7, R7, #-1\n", asm_file);
    fputs("  STR R7, R6, #1\n", asm_file);
    fprintf(asm_file, "  POW_%d\n", func_num);
    fputs("  LDR R7, R6, #1\n", asm_file);
    fputs("  CONST R2, #0\n", asm_file);
    fputs("  CMP R7, R2\n", asm_file);
    fprintf(asm_file, "  BRp POWER_%d\n", func_num);
    fputs("  LDR R7, R6, #-1\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  STR R7, R6, #0\n", asm_file);
    func_num ++ ;
    return func_num;
}

void argN_operation(FILE* asm_file, int arg_num) {
    fprintf(asm_file, "  LDR R7, R5, #%d\n", arg_num +2);
    fputs("  ADD R6, R6, #-1\n", asm_file);
    fputs("  STR R7, R6, #0\n\n", asm_file);
}

void return_function(FILE* asm_file) {
    fputs("  ;;epilogue\n", asm_file);
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  ADD R6, R5, #0\n", asm_file);
    fputs("  ADD R6, R6, #3\n", asm_file);
    fputs("  STR R7, R6, #-1\n", asm_file);
    fputs("  LDR R5, R6, #-3\n", asm_file);
    fputs("  LDR R7, R6, #-2\n", asm_file);
    fputs("  ADD R6, R6, #-1\n", asm_file);
    fputs("  RET\n", asm_file);
}

void if_statement(FILE* asm_file, int final_if, char* label_name) {
    fprintf(asm_file, "  %s_IF_%d\n", label_name, final_if);
    fputs("  LDR R7, R6, #0\n", asm_file);
    fputs("  ADD R6, R6, #1\n", asm_file);
    fputs("  CONST R1, #1\n", asm_file);
    fputs("  CMP R7, R1\n", asm_file);
    fprintf(asm_file, "  BRn , %s_ELSE_%d\n", label_name, final_if);
    
}

void else_statement(FILE* asm_file, int final_if, char* label_name) {
    fprintf(asm_file, "  JMP %s_ENDIF_%d\n", label_name, final_if);
    fprintf(asm_file, "  %s_ELSE_%d\n", label_name, final_if);
}

void endif_noelse_statement(FILE* asm_file, int final_if, char* label_name) {
    fprintf(asm_file, "  %s_ENDIF_%d\n", label_name, final_if);
}