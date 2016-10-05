#include <stdio.h>

void printSCode(char** code){
    printf("source code:\n------------\n");
    int size = sizeof(code)/sizeof(code[0]);
    for(int i=0; i<size; i++)
        printf("%s", char[i]);
}
void printNoComm(char** code){
    printf("source code without comments:\n-----------------------------\n");
    int size = sizeof(code)/sizeof(code[0]);
    for(int i=0; i<size; i++)
        printf("%s", char[i]);
}
void printTokens(struct* token){
    printf("tokens:\n-------\n");
    int numTokens= sizeof(token)/sizeof(token[0]);
    for (int i=0; i<numTokens; i++) // TODO edit parameter names??
        printf("%-13s%d",token.value,token.tokenNum);

}
