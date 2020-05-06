/*Alborz Jelvani 2020*/

#include <stdio.h>
#include <stdlib.h>
#define _DEBUG 1 //1 to print debug 0 for normal
#define _MEMSIZE 256 //size of computers memory in bytes

void fetch(u_int16_t pc);
void execute(u_int16_t inst);
void display();
void load(FILE **fp);
enum registers{
	R_R0=0x0, //0x0
	R_R1,
	R_R2,
	R_R3,
	R_R4,
	R_PC, //program counter
	R_COND, //condition flags: 0x0=default, 0x1 zero, 
	R_INST, //current instruction, not accessible
	R_SP,//stack pointer
	R_COUNT //used for internal purposes
};
enum conditions{
	NOTZERO=0x0,
	ZERO
};
enum instructions{
	OP_HALT=0x0, //0x0
	OP_ADD, //ADD R2 R1 //adds 234 to regist 1 and stores in R1
	OP_CMP, //compare and set zero flag if equal, CMP R1 R2
	OP_LOAD, //LOAD #4354 R0, loads 4354 into register R0
	OP_PUSH, //push to stack, PUSH R2
	OP_POP, //pop from stock, POP R3
	OP_JUMP, //JUMP 0X3244 jump to location
	OP_JUMPZ, //jump if zero flag is set, JUMPZ 0x3244
	OP_JUMPNZ //jump if zero flag is not set, JUMPNZ 0x3244
};

u_int16_t regs[R_COUNT];//access registers
u_int16_t memory[_MEMSIZE]; //bytes of memory, each byte contains an opcode or operand in order
u_int16_t stack[32];
int running = 0;

//sample oparation: 0x30 push R0 onto stack
//sample operation: 0x
int main(int argc, char **argv){
	FILE *input;
	input=fopen(argv[1],"r");
	load(&input);
	running= 1;
	regs[R_PC]=0x0;//set PC to 0
	regs[R_SP]=0x0;//set stack pointer to 0
	/*****sample fibonacci program****
	memory[0]=OP_LOAD;
	memory[1]=1;
	memory[2]=R_R0;
	memory[3]=OP_LOAD;
	memory[4]=1;
	memory[5]=R_R1;
	memory[6]=OP_LOAD;
	memory[7]=1;
	memory[8]=R_R2;
	memory[9]=OP_LOAD;
	memory[10]=0;
	memory[11]=R_R3;
	memory[12]=OP_LOAD;
	memory[13]=10; //nth number to go up to
	memory[14]=R_R4;
	memory[15] = OP_ADD;
	memory[16] = R_R1;
	memory[17] = R_R0;
	memory[18] = OP_ADD;
	memory[19] = R_R0;
	memory[20] = R_R1;
	memory[21] = OP_ADD;
	memory[22] = R_R2;
	memory[23] = R_R3;
	memory[24] = OP_CMP;
	memory[25] = R_R3;
	memory[26] = R_R4;
	memory[27] = OP_JUMPNZ;//jump to back to adding if not equal
	memory[28] = 15; 
	memory[31] = OP_HALT;
	********************/
	
	while(running){
		display();
		fetch(regs[R_PC]);
		execute(regs[R_INST]);
	}
	return EXIT_SUCCESS;
}

void fetch(u_int16_t pc){//loads instruction into instruction register
	regs[R_INST] = memory[pc];
	return;
}

void execute(u_int16_t inst){
	switch(inst){
		case OP_HALT:
			running=0;
			if(_DEBUG) printf("HALT\n");
			break;
		case OP_ADD:
			regs[memory[regs[R_PC]+2]]=regs[memory[regs[R_PC]+1]]+regs[memory[regs[R_PC]+2]];
			regs[R_PC]=regs[R_PC]+3;
			if(_DEBUG) printf("ADD\n");
			break;
		case OP_CMP:
			if((regs[memory[regs[R_PC]+1]]-regs[memory[regs[R_PC]+2]])==0){
				regs[R_COND]=ZERO;
			}else{
				regs[R_COND] = NOTZERO;
			}
			regs[R_PC]=regs[R_PC]+3;
			if(_DEBUG) printf("CMP\n");
			break;
		case OP_LOAD:
			regs[memory[regs[R_PC]+2]]=memory[regs[R_PC]+1];
			regs[R_PC]=regs[R_PC]+3;
			if(_DEBUG) printf("LOAD\n");
			break;
		case OP_PUSH:
			stack[regs[R_SP]]=regs[memory[regs[R_PC]+1]];
			regs[R_SP]++;
			regs[R_PC]=regs[R_PC]+2;
			if(_DEBUG) printf("PUSH\n");
			break;
		case OP_POP:
			regs[R_SP]--;
			regs[memory[regs[R_PC]+1]]=stack[regs[R_SP]];
			regs[R_PC]=regs[R_PC]+2;
			if(_DEBUG) printf("POP\n");
			break;
		case OP_JUMP:
			regs[R_PC]=memory[regs[R_PC]+1];
			if(_DEBUG) printf("JUMP\n");
			break;
		case OP_JUMPZ:
			
			if(regs[R_COND]==ZERO){
				regs[R_PC]= memory[regs[R_PC]+1];
				
				if(_DEBUG) printf("JUMPZ\n");
			}else{
				
				regs[R_PC]=regs[R_PC]+2;
				if(_DEBUG) printf("NO JUMPZ\n");
			}
			break;
		case OP_JUMPNZ:
			
			if(regs[R_COND]==NOTZERO){
				regs[R_PC]= memory[regs[R_PC]+1];
				
				if(_DEBUG) printf("JUMPNZ\n");
			}else{
				
				regs[R_PC]=regs[R_PC]+2;
				if(_DEBUG) printf("NO JUMPNZ\n");
			}
			break;
	}
	return;
}
void display(){
	for(int i=0;i<R_COUNT;i++){
		switch(i){
		case R_R0:
			printf("R0: %d ", regs[i]);
			break;
		case R_R1:
			printf("R1: %d ", regs[i]);
			break;
		case R_R2:
			printf("R2: %d ", regs[i]);
			break;
		case R_R3:
			printf("R3: %d ", regs[i]);
			break;
		case R_R4:
			printf("R4: %d ", regs[i]);
			break;
		case R_PC:
			printf("PC: %d ", regs[i]);
			break;
		case R_COND:
			printf("COND: %d ", regs[i]);
			break;
		case R_INST:
			printf("INST: %x ", regs[i]);
			break;
		case R_SP:
			printf("SP: %d ", regs[i]);
			break;
		}
	}
	printf("\n");
}

void load(FILE **fp){//loads program into memory
	int i=0;
	int dat=0;
	while(fscanf(*fp,"%X",&dat)>0){
		memory[i] = (u_int16_t) dat;
		if(_DEBUG) printf("0x%X\n",dat);
		i++;
	}
	fclose(*fp);
	
}

