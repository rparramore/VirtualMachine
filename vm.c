//
//  main.c
//  vm
//
//  Created by Stephen Ulmer on 9/8/16.
//  Copyright © 2016 Stephen Ulmer. All rights reserved.
//

#include <stdio.h>

#define MAX_STACK_HEIGHT = 2000;
#define MAX_CODE_LENGTH = 500;
#define MAX_LEXI_LEVELS = 3;



typedef struct {
    int op;   // opcode
    int  l;   // L
    int  m;   // M
} Instruction;

int sp; //stack pointer
int bp; //base pointer
int pc; //program counter
struct Instruction ir;


int main(int argc, const char * argv[]) {
    
    sp = 0;
    bp = 1;
    pc = 0;
    ir = 0;
    
    
    int[] stack = new int[MAX_STACK_HEIGHT]; // the stack used in the program
    
    int isNotHalted = 1; //boolean for while loop
    
    
    
    
    //print PL/0 code
    
    
    while(isNotHalted){
        
        //print machine state
        
        pc = pc + 1;
        
        //fetch instruction
            //get operation
                //currentInstruction.op = read(file)
                //currentInstruction.l = read(file)
                //currentInstruction.m = read(file)
        
        
        //execute instruction
            //pass struct instruction
        
    }
        
        
        
        
    return 0;
}
