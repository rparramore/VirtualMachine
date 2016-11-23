#ifndef PARSER_H
#define PARSER_H
#define MAX_SYMBOL_TABLE_SIZE 100

typedef enum {  LIT = 1, OPR = 2, LOD = 3, STO = 4, CAL = 5, INC = 6, JMP = 7,
                JPC = 8, SIO = 9} Instruction;

typedef enum {  RET = 0, NEG = 1, ADD = 2, SUB = 3, MUL = 4, DIV = 5, ODD = 6,
                MOD = 7, EQL = 8, NEQ = 9, LSS = 10, LEQ = 11, GTR = 12,
                GEQ = 13} aInstruction;
                
typedef enum {  CONST = 1, VAR = 2, PROC = 3} SymbolType;
                
typedef struct symbol {
    SymbolType kind;
    char name[12];
    int val;
    int level;
    int addr;
} symbol;

extern symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

Boolean parse(char* fileName);
Boolean eat(Type sym);
void program();
void block();
void constDec();
Boolean constHelper();
void varDec();
Boolean varHelper();
void statement();
void condition();
int isRelOp();
void expression();
void term();
void factor();
int find(char* ident);

#endif