//
//Team Name: Compiler Builder 26
//Stephen Ulmer
//Brandon Royal
//Rebecca Parramore
//Ryan Beasley
//Bryan Hart

#include <stdio.h>

#define MAX_STACK_HEIGHT  2000
#define MAX_CODE_LENGTH  500
#define MAX_LEXI_LEVELS  3



struct Instruction {
    int op;   // opcode
    int  l;   // L
    int  m;   // M
};

int sp; //stack pointer
int bp; //base pointer
int pc; //program counter
struct Instruction ir;

int isNotHalted;//
//
int stack[MAX_STACK_HEIGHT];// the stack used in the program


int main(int argc, const char * argv[]) {
    
    sp = 0;
    bp = 1;
    pc = 0;
    ir.op = 0;
    ir.l = 0;
    ir.m = 0;
    
    isNotHalted = 1; //boolean for while loop
    
    
    
    
    //print PL/0 code
    
    
    while(isNotHalted){
        
        //print machine state
        
        pc = pc + 1;
        
        //fetch instruction
            //get operation
                //ir.op = read(file)
                //ir.l = read(file)
                //ir.m = read(file)
        
        
        //execute instruction
            //pass struct instruction
        
    }
        
        
        
        
    return 0;
}
