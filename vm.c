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


int isNotHalted;//
//
int stack[MAX_STACK_HEIGHT];// the stack used in the program


//operation function declarations
void LIT(struct Instruction instruct);
void OPR(struct Instruction instruct);
void LOD(struct Instruction instruct);
void STO(struct Instruction instruct);
void CAL(struct Instruction instruct);
void INC(struct Instruction instruct);
void JMP(struct Instruction instruct);
void JPC(struct Instruction instruct);
void SIO(struct Instruction instruct);


//helper function for calculating location of base L levels back
int base(int level, int b);




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
    //TODO Stephen
    //many logic and arithmetic functions here
}

//03 LOD function
void LOD(struct Instruction instruct){
    sp = sp + 1;
    stack[sp] = stack[ base(instruct.l, bp) + instruct.m];

}

//04 STO function
void STO(struct Instruction instruct){
    stack[ base(instruct.l, bp) + instruct.m] = stack[ sp ];
    sp = sp - 1;
}

//05 CAL function
void CAL(struct Instruction instruct){
    stack[sp + 1] = 0;           //return value (FV)
    stack[sp + 2] = base(instruct.l, bp); // static link (SL)
    stack[sp + 3] = bp;          // dynamic link (DL)
    stack[sp + 4] = pc;          // return address (RA)
    bp = sp + 1;
    pc = instruct.m;
}

//06 INC function
void INC(struct Instruction instruct){
    sp = sp + instruct.m;
}

//07 JMP function
void JMP(struct Instruction instruct){
    pc = instruct.m;
}

//08 JPC function
void JPC(struct Instruction instruct){
    if(stack[sp]==0){
        pc = instruct.m;
    }
    sp = sp - 1;
}

//09 SIO function
void SIO(struct Instruction instruct){
    //TODO Stephen
}

//base helper function
int base(int level, int b){
    while(level > 0){
        b = stack[b+1];
        level = level - 1;
    }
    return b;
}




int main(int argc, const char * argv[]) {
    sp = 0;
    bp = 1;
    pc = 0;
    
    struct Instruction ir;
    
    ir.op = 0;
    ir.l = 0;
    ir.m = 0;
    
    isNotHalted = 1; //boolean for while loop
    
    
    
    
    //print PL/0 code
    
    
    while(isNotHalted){
        
        // To do Becca: print machine state
        
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
