#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"

int token = 0;
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int numSymbols = 0;
int numVars = 0;
FILE * fp;
int line = 0;

Boolean parse(char* fileName){
    fp = fopen(fileName, "w");
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
    else{
        fprintf(fp, "9 0 2\r\n");
        line++;
    }
}

void block(){
    constDec();
    varDec();
    fprintf(fp, "6 0 %d\r\n", numVars + 4);
    line++;
    statement();
}

void constDec(){
    if(eat(constsym)){
        symbol_table[numSymbols].kind = CONST;
        if(constHelper()){
            numSymbols++;
        }
        while(eat(commasym)){
            symbol_table[numSymbols].kind = CONST;
            if(constHelper()){
                numSymbols++;
            }
        }
        if(!eat(semicolonsym)){
            EnqueueError(&code[token], misssemierr);
        }
    }
}

Boolean constHelper(){
    if (!eat(identsym)){
        EnqueueError(&code[token], varidenterr);
        return FALSE;
    }
    else {
        strcpy(symbol_table[numSymbols].name,code[token-1].val.s);
    }
    if (!eat(eqlsym)){
        EnqueueError(&code[token], idfolleqlerr);
        return FALSE;
    }
    if (!eat(numbersym)){
        EnqueueError(&code[token], eqlbynumerr);
        return FALSE;
    }
    else {
        symbol_table[numSymbols].val = code[token-1].val.d;
    }
    return TRUE;
}

void varDec(){
    if (eat(varsym)){    
        if(varHelper()){
            numSymbols++;
            numVars++;
        }
        while(eat(commasym)){
            if(varHelper()){
                numSymbols++;
                numVars++;
            }
        }
        if(!eat(semicolonsym)){
            EnqueueError(&code[token], misssemierr);
        }
    }    
}

Boolean varHelper(){
    if (!eat(identsym)){
        EnqueueError(&code[token], varidenterr);
        return FALSE;
    }
    else {
        symbol_table[numSymbols].kind = VAR;
        strcpy(symbol_table[numSymbols].name,code[token-1].val.s);
        symbol_table[numSymbols].addr = numSymbols + 4;
    }
}

void ProcedureDec(){
    if(eat(proceduresym))
    {
        
        if (eat(identsym)){
            
            if (eat(semicolonsym))
            {
                
                block();
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
            return FALSE;
        }
    }
}

void statement(){
    Type dummy = code[token].sym;
    int mem;
    int i;
    fpos_t file_pos;
    fpos_t file_pos2;
    
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
            if ((i=find(code[mem].val.s)) != -1){
                fprintf(fp, "4 0 %d\r\n", symbol_table[i].addr);
                line++;
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
            fgetpos(fp, &file_pos);
            fprintf(fp,"\r\n\r\n\r\n\r\n");
            line++;
            if(!eat(thensym)){
                EnqueueError(&code[token], thenexperr);
            }
            statement();
            fsetpos(fp, &file_pos);
            fprintf(fp,"8 0 %d\r\n", line);
            fseek(fp, 0L, SEEK_END);
            break;
        case whilesym:
            token++;
            mem = line;
            condition();
            fgetpos(fp, &file_pos);
            fprintf(fp,"\r\n\r\n\r\n\r\n");
            line++;
            if(!eat(dosym)){
                EnqueueError(&code[token], doexperr);
            }
            statement();
            fprintf(fp, "7 0 %d\r\n", mem);
            line++;
            fsetpos(fp, &file_pos);
            fprintf(fp,"8 0 %d\r\n", line);
            fseek(fp, 0L, SEEK_END);
            break;
        case readsym:
            fprintf(fp, "9 0 1\r\n");
            line++;
            token++;
            if(!eat(identsym)){
                EnqueueError(&code[token-1], idaftercallerr);
            }
            else {
                if ((i=find(code[token-1].val.s)) != -1){
                    fprintf(fp, "4 0 %d\r\n", symbol_table[i].addr);
                    line++;
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
                if ((i=find(code[token-1].val.s)) != -1){
                    if (symbol_table[i].kind == VAR){
                        fprintf(fp, "3 %d %d\r\n", symbol_table[i].level, symbol_table[i].addr);
                    }
                    else if (symbol_table[i].kind == CONST){
                        fprintf(fp, "1 0 %d\r\n", symbol_table[i].val);
                    }
                    line++;
                }
                else {
                    EnqueueError(&code[token-1], cantcallconstvarerr);
                }
                fprintf(fp, "9 0 0\r\n");
                line++;
                if(!eat(semicolonsym)){
                    EnqueueError(&code[token], misssemierr);
                }
                else {
                    token--;
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
            }
            expression();
            fprintf(fp, "2 0 %d\r\n", i);
            line++;
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
    return FALSE;
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
            fprintf(fp, "2 0 2\r\n");
            line++;
        }
        else if (code[token-1].sym == minussym){
            term();
            fprintf(fp, "2 0 3\r\n");
            line++;
        }
    }
}

void term(){
    factor();
    while(eat(multsym) || eat(slashsym)){
        if (code[token-1].sym == multsym){
            factor();
            fprintf(fp, "2 0 4\r\n");
            line++;
        }
        else if (code[token-1].sym == slashsym){
            factor();
            fprintf(fp, "2 0 5\r\n");
            line++;
        }
    }
}

void factor(){
    if(eat(identsym)){
        int i;
        if ((i=find(code[token-1].val.s)) != -1){
            if (symbol_table[i].kind == VAR){
                fprintf(fp, "3 %d %d\r\n", symbol_table[i].level, symbol_table[i].addr);
            }
            else if (symbol_table[i].kind == CONST){
                fprintf(fp, "1 0 %d\r\n", symbol_table[i].val);
            }
            line++;
        }
        else {
            EnqueueError(&code[token-1], cantcallconstvarerr);
        }
    }
    else if(eat(numbersym)){
        fprintf(fp, "1 0 %d\r\n", code[token-1].val.d);
        line++;
    }
    else if(eat(lparentsym)){
        expression();
        if(!eat(rparentsym)){
            EnqueueError(&code[token], missingrparenterr);
        }
    }
}

int find(char* ident){
    for (int i = 0; i < numSymbols; i++){
        if (strcmp(symbol_table[i].name,ident) == 0){
            return i;
        }
    }
    
    return -1;
}