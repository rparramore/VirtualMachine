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

//prints each instruction in format - by Ryan Beasley
void printLine(struct Instruction line, int i);
void printLIT(struct Instruction line, char m[2]);
void printOPR(struct Instruction line);
void printLOD(struct Instruction line, char l[2], char m[2]);
void printSTO(struct Instruction line, char l[2], char m[2]);
void printCAL(struct Instruction line, char l[2], char m[2]);
void printINC(struct Instruction line, char m[2]);
void printJMP(struct Instruction line, char m[2]);
void printJPC(struct Instruction line, char m[2]);
void printSIO(struct Instruction line);

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
    
}

//base helper function
int base(int level, int b){
    while(level > 0){
        b = stack[b+1];
        level = level - 1;
    }
    return b;
}

// Generic printline command that prints the appropriate format for the output of the program. - Ryan Beasley

void printLine(struct Instruction line, int i){
	char stri[4];
	char strl[2];
	char strm[2];
	
	snprintf(stri, 4, "%d", i);
	snprintf(strl, 2, "%d", line.l);
	snprintf(strm, 2, "%d", line.m);

	printf("%2s", stri);

	switch(line.op){
		case 1:
			printLIT(line, strm);
			break;
		case 2:
			printOPR(line);
			break;
		case 3:
			printLOD(line, strl, strm);
			break;
		case 4:
			printSTO(line, strl, strm);
			break;
		case 5:
			printCAL(line, strl, strm);
			break;
		case 6:
			printINC(line, strm);
			break;
		case 7:
			printJMP(line, strm);
			break;
		case 8:
			printJPC(line, strm);
			break;
		case 9:
			printSIO(line);
			break;
	}
}

//Ryan Beasley

void printLIT(struct Instruction line, char m[2]){
	printf(" LIT%10s", m);
}

//Ryan Beasley

void printOPR(struct Instruction line){
	switch(line.m){
		case 0:
			printf(" RET");
			break;
		case 1:
			printf(" NEG");
			break;
		case 2:
			printf(" ADD");
			break;
		case 3:
			printf(" SUB");
			break;
		case 4:
			printf(" MUL");
			break;
		case 5:
			printf(" DIV");
			break;
		case 6:
			printf(" ODD");
			break;
		case 7:
			printf(" MOD");
			break;
		case 8:
			printf(" EQL");
			break;
		case 9:
			printf(" NEQ");
			break;
		case 10:
			printf(" LSS");
			break;
		case 11:
			printf(" LEQ");
			break;
		case 12:
			printf(" GTR");
			break;
		case 13:
			printf(" GEQ");
			break;
	}
}

//Ryan Beasley

void printLOD(struct Instruction line, char l[2], char m[2]){
	printf(" LOD%5s%5s", l, m);
}

//Ryan Beasley

void printSTO(struct Instruction line, char l[2], char m[2]){
	printf(" STO%5s%5s", l, m);
}

//Ryan Beasley

void printCAL(struct Instruction line, char l[2], char m[2]){
	printf(" CAL%5s%5s", l, m);
}

//Ryan Beasley

void printINC(struct Instruction line, char m[2]){
	printf(" INC%10s", m);
}

//Ryan Beasley

void printJMP(struct Instruction line, char m[2]){
	printf(" JMP%10s", m);
}

//Ryan Beasley

void printJPC(struct Instruction line, char m[2]){
	printf(" JPC%10s", m);
}

//Ryan Beasley

void printSIO(struct Instruction line){
	switch(line.m){
		case 0:
			printf(" OUT");
			break;
		case 1:
			printf(" INP");
			break;
		case 2:
			printf(" HLT");
			break;
	}
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
    
    struct Instruction code[MAX_CODE_LENGTH];
    
    //Scans in data from stdin into code[] since we're pipelining our input file and then
    //prints it out in the appropriate format - Ryan Beasley
    int i = 0;
	printf("PL/0 code:\n\n");
	while(1){
		scanf("%d %d %d", &code[i].op, &code[i].l, &code[i].m);
		printLine(code[i], i);
		printf("\n");
		if (code[i].op == 9 && code[i].l == 0 && code[i].m == 2)
			break;
		i++;
	}
    
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
