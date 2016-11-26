#ifndef PARSER_H
#define PARSER_H
#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_CODE_LENGTH 500

typedef enum {  LIT = 1, OPR = 2, LOD = 3, STO = 4, CAL = 5, INC = 6, JMP = 7,
                JPC = 8, SIO = 9
                } Instruction;
                
typedef enum {  RET = 0, NEG = 1, ADD = 2, SUB = 3, MUL = 4, DIV = 5, 
                ODD = 6, MOD = 7, EQL = 8, NEQ = 9, LSS = 10, LEQ = 11, 
                GTR = 12, GEQ = 13                  
                } mInstruction;
                
typedef enum {  CONST = 1, VAR = 2, PROC = 3} SymbolType;
                
typedef struct{
    SymbolType kind;
    char name[12];
    int val;
    int level;
    int addr;
} symbol;

typedef struct{
    Instruction i;
    int l;
    int m;
} PL0;

extern PL0 newCode[MAX_CODE_LENGTH];
extern symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
extern int address;

Boolean parse();
Boolean eat(Type sym);
void program();
void blockI();
void block(char* name);
void constDec();
void constHelper();
void varDec(int* space);
void varHelper(int* space);
void procedureDec();
void statement();
void condition();
int isRelOp();
void expression();
void term();
void factor();
void enter(SymbolType t, char* name, int val, int level, int addr);
int find(char* ident);
void updateProcAddr(char* ident, int proc_addr);
int updateM(int addr, int m);
int gen(Instruction i, int l, int m);
void testParser();
void testSymbolTable();

#endif