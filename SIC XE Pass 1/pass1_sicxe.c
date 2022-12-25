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


int search_opcode(char *a) {
	INST ins;
	char line[100];
	int address = 0, flag=0;
	char temp[20];
	FILE* ptr = fopen("E:/System Software lab/pass1-sicxe/opcode.txt", "r");
	if(a[0] == '+')
	{
		int i=1;
		for(i=1; i<strlen(a) ; i++)
			temp[i-1] = a[i];
		temp[i-1] = '\0';
	}
	else
	{
		strcpy(temp, a);
	}
	while(fgets(line, sizeof(line), ptr)) {
		line[strcspn(line, "\n")] = 0;
		ins = tokenise(line);
		if(strcmp(ins.label, temp) == 0) {
			fclose(ptr);
			int value = strtol(ins.operand, NULL, 10);
			return value;
		}
//		printf("opcode: %s, opnum : %s", ins.opcode, ins.operand);
	}
	fclose(ptr);
	return -1;
}

int search_register(char *a)
{
	FILE* ptr = fopen("E:/System Software lab/pass1-sicxe/registers.txt", "r");
	char line[100], temp[20];
	int i=0;
	INST ins;
	for(i=0; i<strlen(a); i++)
	{
		if(a[i] == ',')
			break;
		temp[i] = a[i];
	}
	temp[i] = '\0';
	
	while(fgets(line, sizeof(line), ptr)) {
		line[strcspn(line, "\n")] = 0;
		ins = tokenise(line);
		if(strcmp(ins.opcode, temp) == 0) {
			fclose(ptr);
			return 1;
		}
//		printf("opcode: %s, opnum : %s", ins.opcode, ins.operand);
	}
	fclose(ptr);
	return 0;
}

int main() {

	char line[100];
//	char error_list[5][100];
	int label_count = 0;

	FILE* f1 = fopen("E:/System Software lab/pass1-sicxe/input.txt", "r");
	FILE* f2 = fopen("E:/System Software lab/pass1-sicxe/intermediate.txt", "w+");
	FILE* ptr = fopen("E:/System Software lab/pass1-sicxe/symtab.txt", "w+");
	fclose(ptr);

	if(f1 == NULL) {
		printf("Unable to open input.txt file\n");
		exit -1;
	}
	if(f2 == NULL) {
		printf("Unable to open intermediate.txt file\n");
		exit -1;
	}
	
//	if(ptr == NULL) {
//		printf("Unable to open symtab.txt file\n");
//		exit -1;
//	}

	INST ins;
	int line_count = 0;
	while(fgets(line, sizeof(line), f1)) {
		line[strcspn(line, "\n")] = 0;
		line_count++;
		ins = tokenise(line);
		if(strcmp(ins.opcode, "END") == 0) {
//			printf("%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
			fprintf(f2, "%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
			break;
		}

		if(strcmp(ins.opcode, "START") == 0) {
			locctr = strtol(ins.operand, NULL, 16);
			startingAddress = strtol(ins.operand, NULL, 16);
			printf("locctr : %X, starting address: %x\n", locctr, startingAddress);
//			printf("%s %s %s\n", ins.label, ins.opcode, ins.operand);
			fprintf(f2, "%s %s %s\n", ins.label, ins.opcode, ins.operand);
			continue;
		}

//		printf("%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
		if(strcmp(ins.opcode, "BASE") == 0)
		{
			fprintf(f2, "     %s %s %s\n", ins.label, ins.opcode, ins.operand);
			continue;
		}
		else
		{
			fprintf(f2, "%x %s %s %s\n",locctr, ins.label, ins.opcode, ins.operand);
		}
			
		

		if(strcmp(ins.label, "") != 0) {
			INST inst1;
			char line1[100];
			int address = 0, flag=0;
			FILE* ptr = fopen("E:/System Software lab/pass1-sicxe/symtab.txt", "a+");
			while(fgets(line1, sizeof(line1), ptr)) {
				line1[strcspn(line1, "\n")] = 0;
				inst1 = tokenise(line1);
				if(strcmp(inst1.opcode, ins.label) == 0) {
					printf("Error Duplicate symbol %s in line : %d\n",ins.label, line_count);
//					strcpy(error_list[error_count], "Error Duplicate symbol in line : %d\n", line_count);
					flag = 1;
					fclose(ptr);
					goto error;
					break;
				}
//		printf("symbol: %s, value : %s", ins.opcode, ins.operand);
			}
			if(flag == 0) {
//				fclose(ptr);
//				FILE* ptr = fopen("E:/System Software lab/symtab.txt", "a+");
				fprintf(ptr,"%s %x\n",ins.label , locctr);
				fclose(ptr);
			}
			
		}
		int value = search_opcode(ins.opcode);
		if( value != -1) {
			if(ins.opcode[0] == '+')
				locctr += 4;
			else 
				locctr += value;
		} else if(strcmp(ins.opcode, "WORD") == 0) {
			locctr += 3;
		} else if(strcmp(ins.opcode, "RESW") == 0) {
			locctr += (3 * strtol(ins.operand, NULL, 10));
		} else if(strcmp(ins.opcode, "RESB") == 0) {
			locctr = locctr + strtol(ins.operand, NULL, 10);
		} else if(strcmp(ins.opcode, "BYTE") == 0) {
			if(ins.operand[0] == 'X') {
				locctr = locctr + ((strlen(ins.operand)-3)/2);
			} else if(ins.operand[0] == 'C') {
				locctr = locctr + (strlen(ins.operand)-3);
			}
		} else {
			printf("Set error flag invalid opcode %s in line %d\n",ins.opcode, line_count);
		}
		error:
			printf("");
	}

	fclose(f1);
	fclose(f2);
//	fclose(ptr);
	

	printf("\nLength of the program = %X\n\n", locctr-startingAddress);
	return 0;
}
