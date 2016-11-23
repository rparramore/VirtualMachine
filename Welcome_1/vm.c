/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_STACK_HEIGHT  2000
#define MAX_CODE_LENGTH  500
#define MAX_LEXI_LEVELS  3




struct Instruction {
    int op;   // opcode
    int  l;   // L
    int  m;   // M
};

void chooseOp(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack, FILE* input);

void LIT(struct Instruction instruct, int* sp, int* stack);
void OPR(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack);
void LOD(struct Instruction instruct, int* sp, int* bp, int* stack);
void STO(struct Instruction instruct, int* sp, int* bp, int* stack);
void CAL(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack);
void INC(struct Instruction instruct, int* sp);
void JMP(struct Instruction instruct, int* pc);
void JPC(struct Instruction instruct, int* sp, int* pc, int* stack);
void SIO(struct Instruction instruct, int* sp, int* stack, FILE* input);
void printLine(struct Instruction line, int i);
void printLIT(struct Instruction line);
void printOPR(struct Instruction line);
void printLOD(struct Instruction line);
void printSTO(struct Instruction line);
void printCAL(struct Instruction line);
void printINC(struct Instruction line);
void printJMP(struct Instruction line);
void printJPC(struct Instruction line);
void printSIO(struct Instruction line);
void printMachineState(int* pc, int* bp, int* sp, int* stack);
int base(int level, int b, int* stack);

int main(int argc, char** argv){
    FILE* fp;
    FILE* input;
    int sp;
    int bp;
    int pc;
    int stack[MAX_STACK_HEIGHT];// the stack used in the program

    sp = 0;
    bp = 1;
    pc = 0;
    fp = fopen(argv[1], "r");
    if (argc == 3){
        input = fopen(argv[2], "r");
    }    
    
    struct Instruction code[MAX_CODE_LENGTH];
    struct Instruction ir;

    ir.op = 0;
    ir.l = 0;
    ir.m = 0;

    int i = 0;
    printf("PL/0 code:\n\n");
    while(1){
        fscanf(fp, "%d %d %d", &code[i].op, &code[i].l, &code[i].m);
        printLine(code[i], i);
        printf("\n");
        if (code[i].op == 9 && code[i].l == 0 && code[i].m == 2)
                break;
        i++;
    }


    printf("\n");
    printf("Execution:\n");
    printf("%23s%5s%5s   stack", "pc", "bp", "sp");
    printf("\n");
    printf("%23s%5s%5s", "0", "1", "0");
    printf("\n");

    i = 0;

    while(1){
        ir.op = code[pc].op;
        ir.l = code[pc].l;
        ir.m = code[pc].m;
        
        printLine(ir, pc);
        
        pc = pc + 1;
        chooseOp(ir, &sp, &bp, &pc, stack, input);

        if(ir.op == 9 && ir.l == 0 && ir.m == 2){
            printMachineState(&sp, &bp, &pc, stack);
            break;
        }
        printMachineState(&sp, &bp, &pc, stack);
    }
}

void chooseOp(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack, FILE* input){
    int operation = instruct.op;
    //switch statement for operations
    switch(operation){
        case 1:
            LIT(instruct, sp, stack);
            break;
        case 2:
            OPR(instruct, sp, bp, pc, stack);
            break;         
        case 3:
            LOD(instruct, sp, bp, stack);
            break;
        case 4:
            STO(instruct, sp, bp, stack);
            break;
        case 5:
            CAL(instruct, sp, bp, pc, stack);
            break;
        case 6:
            INC(instruct, sp);
            break;
        case 7:
            JMP(instruct, pc);
            break;
        case 8:
            JPC(instruct, sp, pc, stack);
            break;
        case 9:
            SIO(instruct, sp, stack, input);
            break;
    }

}

void LIT(struct Instruction instruct, int* sp, int* stack){
	*sp = *sp + 1;
	stack[*sp] = instruct.m;
}

//02 OPR function
void OPR(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack){
    switch(instruct.m){
        // RET
        case 0:
            *sp = *bp - 1; 
            *pc = stack[*sp + 4]; 
            *bp = stack[*sp + 3];
            break;
        // NEG
        case 1:
            stack[*sp] = -stack[*sp];
            break;
        // ADD
        case 2:
            *sp = *sp - 1;
            stack[*sp] = stack[*sp] + stack[*sp + 1];
            break;
        // SUB
        case 3:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] - stack[*sp + 1];
            break;
        // MUL
        case 4:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] * stack[*sp + 1];
            break;
        // DIV
        case 5:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] / stack[*sp + 1];
            break;
        // ODD
        case 6:
            stack[*sp] = stack[*sp] % 2;
            break;
        // MOD
        case 7:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] % stack[*sp + 1];
            break;
        // EQL
        case 8:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] == stack[*sp + 1];
            break;
        // NEQ
        case 9:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] != stack[*sp + 1];
            break;
        // LSS
        case 10:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] <  stack[*sp + 1];
            break;
        // LEQ
        case 11:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] <= stack[*sp + 1];
            break;
        // GTR
        case 12:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] >  stack[*sp + 1];
            break;
        // GEQ
        case 13:
            *sp = *sp - 1; 
            stack[*sp] = stack[*sp] >= stack[*sp + 1];
            break;
    }
}
//03 LOD function
void LOD(struct Instruction instruct, int* sp, int* bp, int* stack){
    *sp = *sp + 1;
    stack[*sp] = stack[ base(instruct.l, *bp, stack) + instruct.m];
}
//04 STO function
void STO(struct Instruction instruct, int* sp, int* bp, int* stack){  
    stack[ base(instruct.l, *bp, stack) + instruct.m] = stack[ *sp ];
    *sp = *sp - 1;
}
//05 CAL function
void CAL(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack){
    stack[*sp + 1] = 0;           //return value (FV)
    stack[*sp + 2] = base(instruct.l, *bp, stack); // static link (SL)
    stack[*sp + 3] = *bp;          // dynamic link (DL)
    stack[*sp + 4] = *pc;          // return address (RA)
    *bp = *sp + 1;
    *pc = instruct.m;
}
//06 INC function
void INC(struct Instruction instruct, int* sp){   
    *sp = *sp + instruct.m;
}
//07 JMP function
void JMP(struct Instruction instruct, int* pc){    
    *pc = instruct.m;
}
//08 JPC function
void JPC(struct Instruction instruct, int* sp, int* pc, int* stack){    
    if(stack[*sp]==0){        
        *pc = instruct.m;    
    }   
    *sp = *sp - 1;
}
//09 SIO function
void SIO(struct Instruction instruct, int* sp, int* stack, FILE* input){
    switch (instruct.m){
        case 0:
            printf("\n%5d%12s", stack[*sp], "");
            *sp = *sp - 1;
            break;
        case 1:
            *sp = *sp + 1;
            fscanf(input, "%d", &stack[*sp]);
            printf("\nRead in %5d%4s", stack[*sp],"");
            break;
        case 2:
            break;
    }
}
//base helper function
int base(int level, int b, int* stack){    
    while(level > 0){        
        b = stack[b+1];        
        level = level - 1;    
    }   
    return b;
}
void printLine(struct Instruction line, int i){

    printf("%3d", i);

    switch(line.op){
        case 1:
            printLIT(line);
            break;
        case 2:
            printOPR(line);
            break;
        case 3:
            printLOD(line);
            break;
        case 4:
            printSTO(line);
            break;
        case 5:
            printCAL(line);
            break;
        case 6:
            printINC(line);
            break;
        case 7:
            printJMP(line);
            break;
        case 8:
            printJPC(line);
            break;
        case 9:
            printSIO(line);
            break;
    }
}

void printLIT(struct Instruction line){
    printf(" LIT%10d", line.m);
}

void printOPR(struct Instruction line){
    switch(line.m){
        case 0:
            printf(" RET%10s", "");
            break;
        case 1:
            printf(" NEG%10s", "");
            break;
        case 2:
            printf(" ADD%10s", "");
            break;
        case 3:
            printf(" SUB%10s", "");
            break;
        case 4:
            printf(" MUL%10s", "");
            break;
        case 5:
            printf(" DIV%10s", "");
            break;
        case 6:
            printf(" ODD%10s", "");
            break;
        case 7:
            printf(" MOD%10s", "");
            break;
        case 8:
            printf(" EQL%10s", "");
            break;
        case 9:
            printf(" NEQ%10s", "");
            break;
        case 10:
            printf(" LSS%10s", "");
            break;
        case 11:
            printf(" LEQ%10s", "");
            break;
        case 12:
            printf(" GTR%10s", "");
            break;
        case 13:
            printf(" GEQ%10s", "");
            break;
    }
}

void printLOD(struct Instruction line){
    printf(" LOD%5d%5d", line.l, line.m);
}

void printSTO(struct Instruction line){
    printf(" STO%5d%5d", line.l, line.m);
}

void printCAL(struct Instruction line){
    printf(" CAL%5d%5d", line.l, line.m);
}

void printINC(struct Instruction line){
    printf(" INC%10d", line.m);
}

void printJMP(struct Instruction line){
    printf(" JMP%10d", line.m);
}

void printJPC(struct Instruction line){
    printf(" JPC%10d", line.m);
}

void printSIO(struct Instruction line){
    switch(line.m){
        case 0:
            printf(" OUT%10s", "");
            break;
        case 1:
            printf(" INP%10s", "");
            break;
        case 2:
            printf(" HLT%10s", "");
            break;
    }
}

void printMachineState(int* sp, int* bp, int* pc, int* stack){
    printf("%6d%5d%5d   ", *pc, *bp, *sp);
    
    int i;
    
    for(i = 1; i <= *sp; i++){
        if (i == *bp && i != 1){
            printf("| ");
        }
        printf("%d ", stack[i]);
    }
    printf("\n");
}