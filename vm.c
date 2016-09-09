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

void LIT(struct Instruction instruct);
void OPR(struct Instruction instruct);
void LOD(struct Instruction instruct);
void STO(struct Instruction instruct);
void CAL(struct Instruction instruct);
void INC(struct Instruction instruct);
void JMP(struct Instruction instruct);
void JPC(struct Instruction instruct);
void SIO(struct Instruction instruct);

//uses opcode to call the function of the corresponding operation
void chooseOp(struct Instruction instruct){
    
    int operation = instruct.op;
    
    //switch statement for operations
    
    switch(operation){
        case 1:
            LIT(instruct);
            break;
            
        case 2:
            OPR(instruct);
            break;
            
        case 3:
            LOD(instruct);
            break;
            
        case 4:
            STO(instruct);
            break;
            
        case 5:
            CAL(instruct);
            break;
            
        case 6:
            INC(instruct);
            break;
            
        case 7:
            JMP(instruct);
            break;
            
        case 8:
            JPC(instruct);
            break;
            
        case 9:
            SIO(instruct);
            break;
            
            
    }
}

//01 LIT function
void LIT(struct Instruction instruct){
    sp = sp + 1;
    stack[sp] = instruct.m;
}

//02 OPR function
void OPR(struct Instruction instruct){
    
}

//03 LOD function
void LOD(struct Instruction instruct){
    
}

//04 STO function
void STO(struct Instruction instruct){
    
}

//05 CAL function
void CAL(struct Instruction instruct){
    
}

//06 INC function
void INC(struct Instruction instruct){
    
}

//07 JMP function
void JMP(struct Instruction instruct){
    
}

//08 JPC function
void JPC(struct Instruction instruct){
    
}

//09 SIO function
void SIO(struct Instruction instruct){
    
}




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
                //ir.op = read(file) OPCODE
                //ir.l = read(file)  LEVEL
                //ir.m = read(file)  MODIFIER
        
        
        //execute instruction
            //pass struct instruction
        
    }
        
        
        
        
    return 0;
}
