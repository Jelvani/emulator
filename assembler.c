#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Assembler*/
/*Alborz Jelvani 2020*/

typedef enum registers{
	R0=0x0, //0x0
	R1,
	R2,
	R3,
	R4,
	PC, //program counter
	COND, //condition flags: 0x0=default, 0x1 zero, 
	INST, //current instruction, not accessible
	SP,//stack pointer
	COUNT //used for internal purposes
} registers;

typedef enum instructions{
	HALT=0x0, //0x0
	ADD, //ADD R2 R1 //adds 234 to regist 1 and stores in R1
	CMP, //compare and set zero flag if equal, CMP R1 R2
	LOAD, //LOAD #4354 R0, loads 4354 into register R0
	PUSH, //push to stack, PUSH R2
	POP, //pop from stock, POP R3
	JUMP, //JUMP 0X3244 jump to location
	JUMPZ, //jump if zero flag is set, JUMPZ 0x3244
	JUMPNZ //jump if zero flag is not set, JUMPNZ 0x3244
} instructions;

struct labels{
	char label[20]; //label name
	int addr; //address of instruction location
};

/*************************************/
registers getregister(char* input);
void printLine(FILE **fp,instructions inst, registers reg1, registers reg2, char* operand, int jmp);
/***************************************/
int main(int argc, char **argv){

	char buffer[1000];
	char opcode[60];
	char operand1[10];
	char operand2[10];
	struct labels label[20];//max 20 labels in assembly
	int lblpt = 0;
	FILE *assembly;
	FILE *output;
	assembly = fopen(argv[1],"r");
	output = fopen("output.txt","w");
	int memoryloc = 0;
	
	registers reg1, reg2;
	instructions inst;
	
    while (fgets(buffer, 1000, assembly) != NULL){
    
    	if(sscanf(buffer,"%s",opcode)!=EOF){
    		
    		if(opcode[0]=='/' && opcode[1]=='/'){
    			continue;
    		}else if(opcode[0]=='.'){
    			 strcpy(label[lblpt].label,opcode);
    			 label[lblpt].addr=memoryloc;
    			lblpt++;
    		}else{
    			if(strcmp(opcode,"HALT")==0){
    				inst=HALT;
    				memoryloc+=1;
    				printLine(&output,inst,reg1,reg2, operand1,0);
    			}else if(strcmp(opcode,"ADD")==0){
    				sscanf(buffer,"%*s %s %s",operand1,operand2);
    				inst=ADD;
    				reg1=getregister(operand1);
    				reg2=getregister(operand2);
    				memoryloc+=3;
    				printLine(&output,inst,reg1,reg2, operand1,0);
    			}else if(strcmp(opcode,"CMP")==0){
    				sscanf(buffer,"%*s %s %s",operand1,operand2);
    				inst=CMP;
    				reg1=getregister(operand1);
    				reg2=getregister(operand2);
    				memoryloc+=3;
    				printLine(&output,inst,reg1,reg2, operand1,0);
    			}else if(strcmp(opcode,"LOAD")==0){
    				sscanf(buffer,"%*s %s %s",operand1,operand2);
    				inst=LOAD;
    				reg1=getregister(operand2);
    				memoryloc+=3;
    				printLine(&output,inst,reg1,reg2, operand1,0);
    			}else if(strcmp(opcode,"PUSH")==0){
    				sscanf(buffer,"%*s %s",operand1);
    				inst=PUSH;
    				reg1=getregister(operand1);
    				memoryloc+=2;
    				printLine(&output,inst,reg1,reg2, operand1,0);
    			}else if(strcmp(opcode,"POP")==0){
    				sscanf(buffer,"%*s %s",operand1);
    				inst=POP;
    				reg1=getregister(operand1);
    				memoryloc+=2;
    				printLine(&output,inst,reg1,reg2, operand1,0);
    			}else{
    				
    				if(strcmp(opcode,"JUMP")==0){
    				sscanf(buffer,"%*s %s",operand1);
					inst=JUMP;
    				memoryloc+=2;
					}else if(strcmp(opcode,"JUMPZ")==0){
						sscanf(buffer,"%*s %s",operand1);
						inst=JUMPZ;
						memoryloc+=2;
					}else if(strcmp(opcode,"JUMPNZ")==0){
						sscanf(buffer,"%*s %s",operand1);
						inst=JUMPNZ;
						memoryloc+=2;
					}
					for(int i=0;i<lblpt;i++){
						
    					if(strcmp(label[i].label,operand1)==0){
    						printLine(&output,inst,reg1,reg2, operand1,label[i].addr);
    						break;
    					}else{
    						printf("Assembler error: label %s not found!\n",operand1);
    					}
    				}
    			}
    			
    		}
    	}
    } 
		
	fclose(assembly);
	fclose(output);
	return 1;
}
registers getregister(char* input){

		if(strcmp(input,"R0")==0){
			return R0;
    	}else if(strcmp(input,"R1")==0){
    		return R1;
    	}else if(strcmp(input,"R2")==0){
    		return R2;
    	}else if(strcmp(input,"R3")==0){
    		return R3;
    	}else if(strcmp(input,"R4")==0){
    		return R4;
    	}
}

void printLine(FILE **fp,instructions inst, registers reg1, registers reg2, char* operand, int jmp){
	switch(inst){
		case HALT:
			fprintf(*fp,"0x%X\n",inst);
			break;
		case ADD:
			fprintf(*fp,"0x%X 0x%X 0x%X\n",inst,reg1, reg2);
			break;
		case CMP:
			fprintf(*fp,"0x%X 0x%X 0x%X\n",inst,reg1, reg2);
			break;
		case LOAD:
			fprintf(*fp,"0x%X 0x%X 0x%X\n",inst,atoi(operand), reg1);
			break;
		case PUSH:
			fprintf(*fp,"0x%X 0x%X\n",inst,reg1);
			break;
		case POP:
			fprintf(*fp,"0x%X 0x%X\n",inst,reg1);
			break;
		case JUMP:
			fprintf(*fp,"0x%X 0x%X\n",inst,jmp);
			break;
		case JUMPZ:
			fprintf(*fp,"0x%X 0x%X\n",inst,jmp);
			break;
		case JUMPNZ:
			fprintf(*fp,"0x%X 0x%X\n",inst,jmp);
			break;
	}
}
