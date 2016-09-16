#include <stdio.h>

#define MAX_STACK_HEIGHT  2000
#define MAX_CODE_LENGTH  500
#define MAX_LEXI_LEVELS  3

struct Instruction {
    int op;   // opcode
    int  l;   // L
    int  m;   // M
};

void chooseOp(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack);
void LIT(struct Instruction instruct, int* sp, int* stack);
void OPR(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack);
void LOD(struct Instruction instruct, int* sp, int* bp, int* stack);
void STO(struct Instruction instruct, int* sp, int* bp, int* stack);
void CAL(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack);
void INC(struct Instruction instruct, int* sp);
void JMP(struct Instruction instruct, int* pc);
void JPC(struct Instruction instruct, int* sp, int* pc, int* stack);
void SIO(struct Instruction instruct);

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

void printMachineState(struct Instruction line, int* pc, int* bp, int* sp, int* stack);

int base(int level, int b, int* stack);

int main(void){
  int sp;
	int bp;
	int pc;
	
	int stack[MAX_STACK_HEIGHT];// the stack used in the program

	sp = 0;
	bp = 1;
	pc = 0;

	struct Instruction code[MAX_CODE_LENGTH];
	struct Instruction ir;

	ir.op = 0;
	ir.l = 0;
	ir.m = 0;

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
		
		pc = pc + 1;

		if(ir.op == 9 && ir.l == 0 && ir.m == 2){
			printMachineState(ir, &sp, &bp, &pc, stack);
			break;
		}

		chooseOp(ir, &sp, &bp, &pc, stack);
		printMachineState(ir, &sp, &bp, &pc, stack);
		
	}
}

void chooseOp(struct Instruction instruct, int* sp, int* bp, int* pc, int* stack){
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
			SIO(instruct);
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
		case 0:
			*sp = *bp - 1; 
			*pc = stack[*sp + 4]; 
			*bp = stack[*sp + 3];
			break;
		case 1:
			stack[*sp] = -stack[*sp];
			break;
		case 2:
			*sp = *sp - 1;
			stack[*sp] = stack[*sp] + stack[*sp + 1];
			break;
		case 3:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] - stack[*sp + 1];
			break;
		case 4:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] * stack[*sp + 1];
			break;
		case 5:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] / stack[*sp + 1];
			break;
		case 6:
			stack[*sp] = stack[*sp] % 2;
			break;
		case 7:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] % stack[*sp + 1];
			break;
		case 8:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] == stack[*sp + 1];
			break;
		case 9:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] != stack[*sp + 1];
			break;
		case 10:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] <  stack[*sp + 1];
			break;
		case 11:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] <= stack[*sp + 1];
			break;
		case 12:
			*sp = *sp - 1; 
			stack[*sp] = stack[*sp] >  stack[*sp + 1];
			break;
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

void SIO(struct Instruction instruct){
    
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
	char stri[4];
	char strl[2];
	char strm[2];
	
	snprintf(stri, 4, "%d", i);
	snprintf(strl, 2, "%d", line.l);
	snprintf(strm, 2, "%d", line.m);

	printf("%3s", stri);

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

void printLIT(struct Instruction line, char m[2]){
	printf(" LIT%10s", m);
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

void printLOD(struct Instruction line, char l[2], char m[2]){
	printf(" LOD%5s%5s", l, m);
}

void printSTO(struct Instruction line, char l[2], char m[2]){
	printf(" STO%5s%5s", l, m);
}

void printCAL(struct Instruction line, char l[2], char m[2]){
	printf(" CAL%5s%5s", l, m);
}

void printINC(struct Instruction line, char m[2]){
	printf(" INC%10s", m);
}

void printJMP(struct Instruction line, char m[2]){
	printf(" JMP%10s", m);
}

void printJPC(struct Instruction line, char m[2]){
	printf(" JPC%10s", m);
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

void printMachineState(struct Instruction line, int* sp, int* bp, int* pc, int* stack){
	char strpc[4];
	char strbp[4];
	char strsp[4];
	
	snprintf(strpc, 4, "%d", *pc);
	snprintf(strbp, 4, "%d", *bp);
	snprintf(strsp, 4, "%d", *sp);

	printLine(line, *pc);	
	
	printf("%6s%5s%5s   ", strpc, strbp, strsp);

	int i;

	for(i = 1; i <= *sp; i++){
		printf("%d ", stack[i]);
	}
	
	printf("\n");
}
