#include<stdio.h>
#include<Stdlib.h>
#include<string.h>

int locctr = 0;
int startingAddress = 0;
struct Instruction {
	char label[20];
	char opcode[20];
	char operand[50];
} typedef INST;

int line_count = 0;

INST tokenise(char line[]) {
	int count=0;
	INST inst;
	char *token1="";
	char *token2=NULL;
	char *token3="";

	token1=strtok(line," ");
	token2=strtok(NULL," ");
	token3=strtok(NULL," ");

	if(token2==NULL)
		count=1;

	if(token3==NULL && token2!=NULL)
		count=2;

	if(token3!=NULL)
		count=3;

	if(count==1) {
		strcpy(inst.label, "");
		strcpy(inst.opcode, token1);
		strcpy(inst.operand, "");
	}

	if(count==2) {
		strcpy(inst.label, "");
		strcpy(inst.opcode, token1);
		strcpy(inst.operand, token2);
	}

	if(count==3) {
		strcpy(inst.label, token1);
		strcpy(inst.opcode, token2);
		strcpy(inst.operand, token3);
	}
	return inst;
}



int duplicate_symbol(char *var)
{
	FILE* ptr = fopen("E:/System Software lab/pass1-avr/symtab.txt", "r");
	char line1[100];
	INST inst1;
	while(fscanf(ptr,"%s %s",inst1.opcode, inst1.operand) != EOF)
	{
		//printf("\n opcode: %s", inst1.opcode);
		if(strcmp(inst1.opcode, var) == 0) {
			printf("\nError Duplicate symbol %s in line : %d\n",inst1.opcode, line_count);
//					strcpy(error_list[error_count], "Error Duplicate symbol in line : %d\n", line_count);
			fclose(ptr);
			return 1;
			break;
		}
	}
	
	fclose(ptr);
	return 0;
}

int main() {

	char line[100];
//	char error_list[5][100];

	int label_count = 0;

	FILE* f1 = fopen("input.txt", "r");
	FILE* f2 = fopen("E:/System Software lab/pass1-avr/intermediate.txt", "w+");
	FILE* ptr = fopen("E:/System Software lab/pass1-avr/symtab.txt", "w+");

	if(f1 == NULL) {
		printf("Unable to open input.txt file\n");
		exit -1;
	}
	if(f2 == NULL) {
		printf("Unable to open intermediate.txt file\n");
		exit -1;
	}

	INST ins;
//	int line_count = 0;
	while(fgets(line, sizeof(line), f1)) {
		line[strcspn(line, "\n")] = 0;
		line_count++;
		ins = tokenise(line);
		
		if(strcmp(ins.opcode, ".ORG") == 0)
		{
			char op[20];
			int i=0;
			for(i=2; i<strlen(ins.operand) ; i++)
			{
				op[i-2] = ins.operand[i];
			}
			
			locctr = strtol(op, NULL, 16);
			fprintf(f2, "%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
			printf("locctr : %x\n", locctr);
			op[0] = '\0';
		}
		else if(strcmp(ins.label, ".EQU") == 0)
		{
			fclose(ptr);
			if(duplicate_symbol(ins.opcode) == 0)
			{
				char op[20];
				int i=0;
				for(i=3; i<strlen(ins.operand) ; i++)
				{
					op[i-3] = ins.operand[i];
				}
				op[i-3]=0;
//				printf("%s %s\n", ins.opcode, op);
				ptr = fopen("E:/System Software lab/pass1-avr/symtab.txt", "a+");
				fprintf(ptr, "%s %s\n", ins.opcode, op);
				fprintf(f2, "%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
				op[0] = '\0';
			}
			
		}
		else
		{
			if(strcmp(ins.opcode, "STS") ==0 || strcmp(ins.opcode, "LTS") ==0)
			{
//				printf("%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
				fprintf(f2, "%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
				locctr += 4;
			}
			else
			{
//				printf("%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
				fprintf(f2, "%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
				locctr += 2;
			}
		}
		
	}
				
	fclose(f1);
	fclose(f2);
	fclose(ptr);
	
//	printf("\nLength of the program = %X\n\n", locctr-startingAddress);
	return 0;
}
