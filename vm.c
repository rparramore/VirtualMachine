//Team Name: Compiler Builder 26
//Stephen Ulmer
//Rebecca parramore
//Brandon Royal
//Ryan Beasley
//Bryan Hart

#include <stdio.h>

#define MAX_STACK_HEIGHT = 2000;
#define MAX_CODE_LENGTH = 500;
#define MAX_LEXI_LEVELS = 3;



struct {
    int op;   // opcode
    int  l;   // L
    int  m;   // M
} instruction;


int main(int argc, const char * argv[]) {
    
    int sp = 0; //stack pointer
    int bp = 1; //base pointer
    int pc = 0; //program counter
    int ir = 0; //instruction register
    
    int[] stack = new int[MAX_STACK_HEIGHT];
    
    int isNotHalted = 1; //boolean for while loop
    
    
    while(isNotHalted){
        
        pc = pc + 1;
        
        //fetch instruction
            //get operation
        
        
        //execute instruction
        
    }
        
        
        
        
    return 0;
}
