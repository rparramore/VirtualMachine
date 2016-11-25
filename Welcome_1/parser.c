#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"

int token = 0;
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int numSymbols = 0;
int numVars = 0;
int address;
int level = -1;
PL0 newCode[MAX_CODE_LENGTH];

Boolean parse(){
    address = 0;
    program();
    if (front == NULL){
        return TRUE;
    }
    return FALSE;
}

Boolean eat(Type sym){    
    if (code[token].sym == sym){
        token++;
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void program(){
    block();
    if(!eat(periodsym))
        EnqueueError(&code[token], perexperr);
}

int block(){
    int space = 4;
    int prevNumSymbols;
    int jumpaddr;
    int proc_addr;
    level++;
    prevNumSymbols = numSymbols;
    
    jumpaddr = gen(JMP, 0, 0);
    
    constDec();
    varDec(&space);  
    procedureDec();
    
    updateM(jumpaddr, address);      
    proc_addr = gen(INC, 0, space); 
    
    statement();
    gen(OPR, 0, RET);
    
    numSymbols = prevNumSymbols;
    level--;
    return proc_addr;
}

void constDec(){
    if(eat(constsym)){
        
        constHelper();
        while(eat(commasym)){
            symbol_table[numSymbols].kind = CONST;
            constHelper();
        }
        if(!eat(semicolonsym)){
            EnqueueError(&code[token], misssemierr);
        }
    }
}

void constHelper(){
    char name[MAX_IDENT_SIZE];
    if (eat(identsym)){
        strncpy(name, code[token-1].val.s, sizeof(name));
        if (eat(eqlsym)){            
            if (eat(numbersym)){                
                enter(CONST, name, code[token-1].val.d, level, 0);
                numSymbols++;
            }
            else {
                EnqueueError(&code[token], eqlbynumerr);
                return FALSE;                
            }
        }
        else{
            EnqueueError(&code[token], idfolleqlerr);
            return FALSE;
        }        
    }
    else {        
        EnqueueError(&code[token], varidenterr);
        return FALSE;
    }
    
    
    return TRUE;
}

void varDec(int* space){
    if (eat(varsym)){    
        varHelper(space);
        while(eat(commasym)){
            varHelper(space);
        }
        if(!eat(semicolonsym)){
            EnqueueError(&code[token], misssemierr);
        }        
    }       
}

void varHelper(int* space){
    if (eat(identsym)){
        enter(VAR, code[token-1].val.s, 0, level, (*space)) //check space pointer later
        (*space)++;
    }
    else {
        EnqueueError(&code[token], varidenterr);
    }
}

void procedureDec(){
    int proc_addr;
    char name[MAX_IDENT_SIZE];
    if(eat(procsym))
    {        
        if (eat(identsym)){
            name = strncpy(name, code[token-1].val.s, sizeof(name));
            enter(PROC, name, 0, level, 666);
            if (eat(semicolonsym))
            {                
                proc_addr = block();
                updateProcAddr(name, proc_addr);
                if(!eat(semicolonsym)){
                    EnqueueError(&code[token], misssemierr);
                }
                
            }
            else
                EnqueueError(&code[token], misssemierr);
        }
        else
        {
            EnqueueError(&code[token], varidenterr);
        }
    }
}

void statement(){
    Type dummy = code[token].sym;
    int mem;
    int mem2;
    int i;
    
    switch(dummy){
        case identsym:
            mem = token;
            token++;
            if(!eat(becomessym)){
                if (eat(eqlsym)){
                    EnqueueError(&code[token], wrongeqerr);
                }
                else{
                    EnqueueError(&code[token], assignoperexperr);
                }
            }
            expression();
            if ((i=find(code[mem].val.s)) != -1 && symbol_table[i].kind != PROC){
                gen(STO, symbol_table[i].level, symbol_table[i].addr);
            }
            else {
                EnqueueError(&code[token-1], cantcallconstvarerr);
            }
            if(!eat(semicolonsym)){
                EnqueueError(&code[token], misssemierr);
            }
            else {
                token--;
            }
            break;
        case beginsym:
            token++;
            statement();
            while (eat(semicolonsym)){
                statement();
            }
            if(!eat(endsym)){
                EnqueueError(&code[token], incorrsymaftstaterr);
            }
            break;
        case ifsym:
            token++;                      
            condition();
            mem = gen(JPC, 0, 0);
            if(!eat(thensym)){
                EnqueueError(&code[token], thenexperr);
            }
            statement();
            updateM(mem, address);
            break;
        case whilesym:
            token++;
            mem = address;
            condition();
            mem2 = gen(JPC, 0, 0);
            if(!eat(dosym)){
                EnqueueError(&code[token], doexperr);
            }
            statement();
            gen(JMP, 0, mem);
            updateM(mem2, address);
            break;
        case readsym:
            gen(SIO, 0, 1);
            token++;
            if(!eat(identsym)){
                EnqueueError(&code[token-1], idaftercallerr);
            }
            else {
                if ((i=find(code[token-1].val.s)) != -1 && symbol_table[i].kind != PROC){
                    gen(STO, symbol_table[i].level, symbol_table[i].addr);
                }
                else {
                    EnqueueError(&code[token-1], cantcallconstvarerr);
                }
            }
            if(!eat(semicolonsym)){
                EnqueueError(&code[token], misssemierr);
            }
            else {
                token--;
            }
            break;
        case writesym:
            token++;
            if(!eat(identsym)){
                EnqueueError(&code[token], idaftercallerr);
            }
            else {                
                if ((i=find(code[token-1].val.s)) != -1 && symbol_table[i].kind != PROC){
                    if (symbol_table[i].kind == VAR){
                        gen(LOD, symbol_table[i].level, symbol_table[i].addr);
                    }
                    else if (symbol_table[i].kind == CONST){
                        gen (LIT, 0, symbol_table[i].val);
                    }
                }
                else {
                    EnqueueError(&code[token-1], cantcallconstvarerr);
                }
                gen(SIO, 0, 0);
                if(!eat(semicolonsym)){
                    EnqueueError(&code[token], misssemierr);
                }
                else {
                    token--;
                }
            }
        case callsym:
            token++;
            if (!eat(identsym)){
                EnqueueError(&code[token], idaftercallerr);
            }
            else{
                if((i=find(code[token-1].val.s)) != -1 && symbol_table[i].kind == PROC){
                    gen(CAL, symbol_table[i].level, symbol_table[i].addr);
                }
                else {
                    EnqueueError(&code[token-1], idaftercallprocerr);
                }
            }
            break;
    }
}

void condition(){
    Type dummy = code[token].sym;
    token++;
    int i;
    switch(dummy){
        case oddsym:
            expression();            
            break;
        default:
            token--;
            expression();
            if((i=isRelOp()) == -1){
                EnqueueError(&code[token], exprelaterr);
                expression();
            }
            else {
                expression();
                gen(OPR, 0, i);
            }            
            break;
    }
}

int isRelOp(){
    Type dummy = code[token].sym;
    token++;
    switch(dummy){
        case eqlsym:
            return 8;
            break;
        case neqsym:
            return 9;
            break;
        case lessym:
            return 10;
            break;
        case leqsym:
            return 11;
            break;
        case gtrsym:
            return 12;
            break;
        case geqsym:
            return 13;
            break;
        default:
            return -1;
    }
}

void expression(){
    if(eat(plussym)){
        
    }
    if(eat(minussym)){
        
    }
    term();
    while(eat(plussym) || eat(minussym)){
        if (code[token-1].sym == plussym){
            term();
            gen(OPR, 0, ADD);
            address++;
        }
        else if (code[token-1].sym == minussym){
            term();
            gen(OPR, 0, NEG);
            address++;
        }
    }
}

void term(){
    factor();
    while(eat(multsym) || eat(slashsym)){
        if (code[token-1].sym == multsym){
            factor();
            gen(OPR, 0, MUL);
        }
        else if (code[token-1].sym == slashsym){
            factor();
            gen(OPR, 0, DIV);
        }
    }
}

void factor(){
    if(eat(identsym)){
        int i;
        if ((i=find(code[token-1].val.s)) != -1){
            if (symbol_table[i].kind == VAR){
                gen(LOD, symbol_table[i].level, symbol_table[i].addr);
            }
            else if (symbol_table[i].kind == CONST){
                gen(LIT, 0, symbol_table[i].val);
            }
            else {
                EnqueueError(&code[token-1], cantcallconstvarerr);
            }
        }
        else {
            EnqueueError(&code[token-1], cantcallconstvarerr);
        }
    }
    else if(eat(numbersym)){
        gen(LIT, 0, code[token-1].val.d);
    }
    else if(eat(lparentsym)){
        expression();
        if(!eat(rparentsym)){
            EnqueueError(&code[token], missingrparenterr);
        }
    }
}

void enter(SymbolType t, char* name, int val, int level, int addr){
    symbol_table[numSymbols].kind = t;
    strncpy(symbol_table[numSymbols].name, name, sizeof(name));
    symbol_table[numSymbols].val = val;
    symbol_table[numSymbols].addr = addr;
    numSymbols++;
}

int find(char* ident){
    for (int i = numSymbols-1; i >= 0; i--){
        if (strcmp(symbol_table[i].name,ident) == 0){
            return i;
        }
    }    
    return -1;
}

void updateProcAddr(char* ident, int proc_addr){
    int i;
    i = find(ident);
    symbol_table[i].addr = proc_addr;
}

int updateM(int addr, int m){
    newCode[addr].m = m;
}

int gen(Instruction i, int l, int m){
    newCode[address].i = i;
    newCode[address].l = l;
    newCode[address].m = m;
    address++;
    return address-1;
}

