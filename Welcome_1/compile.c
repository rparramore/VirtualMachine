#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
//comment
char* getName(char* fileName);

int main(int argc, char* argv[]){
    lex(argc, argv);
    
    if(parse(getName(argv[1]))){
        printf("No errors, program is syntactically correct");
    }
    else{
        PrintErrors(argv[1]);
    }   
}

char* getName(char* fileName){
    int size = strlen(fileName);
    char* temp = (char*) malloc(size * sizeof(char));
    int i;
    
    for (i = 0; *(fileName+i) != '.'; i++){
        *(temp+i) = *(fileName+i);
    }
    
    *(temp+i) = '.';
    *(temp+i+1) = 'p';
    *(temp+i+2) = 'm';
    *(temp+i+3) = '0';
    *(temp+i+4) = '\0';
    
    return temp;
}