#include<stdio.h>
#include<Stdlib.h>
#include<string.h>

struct Instruction {
	char label[20];
	char opcode[20];
	char operand[50];
	char address[20];
} typedef INST;

INST tokenise(char line[]) {
	int count=0;
	INST inst;
	char *token1="";
	char *token2="";
	char *token3=NULL;
	char *token4=NULL;

	token1=strtok(line," ");
	token2=strtok(NULL," ");
	token3=strtok(NULL," ");
	token4=strtok(NULL," ");

	if(token2==NULL)
		count=1;

	if(token3==NULL && token2!=NULL)
		count=2;

	if(token3!=NULL && token4==NULL)
		count=3;

	if(token4!=NULL)
		count=4;

	if(count==1) {
		strcpy(inst.address, "");
		strcpy(inst.label, "");
		strcpy(inst.opcode, "");
		strcpy(inst.operand, "");
	}

	if(count==2) {
		strcpy(inst.address, token1);
		strcpy(inst.label, "");
		strcpy(inst.opcode, token2);
		strcpy(inst.operand, "");
	}

	if(count==3) {
		strcpy(inst.address, token1);
		strcpy(inst.label, "");
		strcpy(inst.opcode, token2);
		strcpy(inst.operand, token3);
	}

	if(count==4) {
		strcpy(inst.address, token1);
		strcpy(inst.label, token2);
		strcpy(inst.opcode, token3);
		strcpy(inst.operand, token4);
	}
	return inst;
}

char* search_opcode(char *a) {
	INST ins;
	char line[100];
	int address = 0, flag=0;
	char *ans=(char*)malloc(10*sizeof(char));
	FILE* ptr = fopen("E:/System Software lab/pass2/optab.txt", "r");
	while(fgets(line, sizeof(line), ptr)) {
		line[strcspn(line, "\n")] = 0;
		ins = tokenise(line);
//		printf("opcode: %s, opnum : %s\n", ins.address, ins.opcode);
		if(strcmp(ins.address, a) == 0) {
			fclose(ptr);
			strcpy(ans, ins.opcode);
			return ans;
		}
		
	}
	fclose(ptr);
	return NULL;
}

char* search_symtab(char *a) {
	INST ins;
	char line[100];
	int address = 0, flag=0;
	char *ans=(char*)malloc(10*sizeof(char));
	FILE* ptr = fopen("E:/System Software lab/pass2/symtab.txt", "r");
	while(fgets(line, sizeof(line), ptr)) {
		line[strcspn(line, "\n")] = 0;
		ins = tokenise(line);
		printf("symbol: %s, value : %s\n", ins.address, ins.opcode);
		if(strcmp(ins.address, a) == 0) {
			fclose(ptr);
			strcpy(ans, ins.opcode);
			return ans;
		}
		
	}
	fclose(ptr);
	return NULL;
}

int main()
{
//	printf(search_symtab("ZERO"));
//	printf(search_opcode("LDX"));

	FILE* f1 = fopen("E:/System Software lab/pass2/intermediate1.txt", "r");
	FILE* f2 = fopen("E:/System Software lab/pass2/objectprog.txt", "w+");
	FILE* f3 = fopen("E:/System Software lab/pass2/output.txt", "w+");
	
	int line_count=0;
	char line[100];
	char text[200];
	INST ins;
	int i=0;

	int len=37;
	int textlen=0, flag=0;
	int startingAddress=0;
//	char obj[100];
//	obj[0] = 0;
	char temp[100];

	if(f1 == NULL) {
		printf("Unable to open input.txt file\n");
		exit -1;
	}
	if(f2 == NULL) {
		printf("Unable to open intermediate.txt file\n");
		exit -1;
	}

	if(f3 == NULL) {
		printf("Unable to open input.txt file\n");
		exit -1;
	}
	
//	char *ch = search_opcode("LDX");
//	if(ch != NULL)
//		printf("The code is %s\n", ch);
//	else
//		printf("Invalid opcode");
	while(fgets(line, sizeof(line), f1)) 
	{
		line[strcspn(line, "\n")] = 0;
		line_count++;
		ins = tokenise(line);
		printf("%s %s %s %s\n",ins.address, ins.label, ins.opcode, ins.operand);
//		fprintf(f3, "%s %s %s %s\n",ins.address, ins.label, ins.opcode, ins.operand);
		
		if(line_count == 1) {
			if(strcmp(ins.opcode, "START") == 0) {
				fprintf(f3, "%s %s %s %s\n",ins.address, ins.label, ins.opcode, ins.operand);
				startingAddress = strtol(ins.operand, NULL, 16);
				fprintf(f2, "H^%6s^%6s^%6x\n",ins.address, ins.operand, len);
				strcpy(text, "T^00");
				strcat(text, ins.operand);
				strcat(text, "^--");
//				printf("starting address: %x", startingAddress);
//				printf("%s\n", text);
//				fprintf(f3, "T^%6s^--", ins.operand);
				continue;
			} else {
				fprintf(f2, "H^      ^000000^%6x\n",len);
				strcpy(text, "T^000000^--");
//				fprintf(f3, "T^000000^--", );
			}	
		}
		
		if(strcmp(ins.opcode, "END") == 0)
		{
			fprintf(f3, "%s %s %s %s\n",ins.address, ins.label, ins.opcode, ins.operand);
			if(textlen != 0)
			{
				char buffer[5];
				sprintf(buffer,"%x",textlen);
				if(strlen(buffer)==1)
				{
					text[10] = buffer[0];
				}
				else
				{
					text[9] = buffer[0];
					text[10] = buffer[1];
				}
				fprintf(f2, "%s\n",text);
			}
			fprintf(f2, "E^%x\n",startingAddress);
			break;
		}
		
		char* opc = search_opcode(ins.opcode);
		char obj[100];
		if(opc != NULL) {
			textlen += 3;
			strcat(obj, opc);
			printf("opcode: %s , object code: %s\n", opc, obj);
			if(strcmp(ins.operand, "") != 0) {
				flag = 0;
				for(i=0; i<strlen(ins.operand); i++) {
					if(ins.operand[i] == ',') {
						flag = 1;
						break;
					}
					temp[i] = ins.operand[i];
				}
				temp[i]='\0';

				opc = search_symtab(temp);
				printf("symbol value: %s\n", opc);
				if(opc != NULL) {
//						char ch = opc[0];
					if(flag == 1) {
						char buffer[5];
						buffer[0] = opc[0];
    					buffer[1] = 0;
						int x = strtol(buffer, NULL, 16);
						x = x|8;
						sprintf(buffer,"%x",x);
    					opc[0] = buffer[0];
					}

					strcat(obj, opc);
				} else {
					strcat(obj, "0000");
					printf("Set error flag\n");
				}


			} 
			else {
				strcat(obj, "0000");
			}
			
//			printf("object code: %s\n", obj);
//			obj[0] = '\0';
		}
		else if(strcmp(ins.opcode, "BYTE")==0) {
			if(ins.operand[0] == 'X') {
				printf("operand: %s\n", ins.operand);
				for(i=2 ; i<strlen(ins.operand)-1; i++)
					temp[i-2] = ins.operand[i];

				temp[i-2]='\0';
				strcat(obj, temp);
				textlen += ((strlen(ins.operand)-3)/2);
			} else if(ins.operand[0] == 'C') {
				for(i=2 ; i<strlen(ins.operand)-1; i++) {
					int x = (int)ins.operand[i];
					char buffer[20];
					itoa(x, buffer, 16);
					strcat(obj, buffer);
				}
				textlen += (strlen(ins.operand)-3);
			}
//			printf("object code: %s\n", obj);
//			obj[0] = '\0';
		}
		else if(strcmp(ins.opcode, "WORD") ==0)
		{
			textlen += 3;
			char buffer[10];
			int x = strtol(ins.operand, NULL, 10);
//			int x=30;
			sprintf(buffer,"%6x",x);
			for(i=0; i<strlen(buffer); i++)
			{
				if(buffer[i] == ' ')
					buffer[i] = '0';
			}
			strcat(obj, buffer);
//			printf("object code: %s\n", obj);
//			obj[0] = '\0';
		}
		else
		{
			fprintf(f3, "%s %s %s %s\n",ins.address, ins.label, ins.opcode, ins.operand);
			continue;
		}
		printf("object code: %s\n", obj);
//		obj[0] = '\0';

		fprintf(f3, "%s %s %s %s %s\n",ins.address, ins.label, ins.opcode, ins.operand, obj);
		
		if(textlen <= 30)
		{
			printf("object code: %s\n", obj);
			printf("%s\n",obj);
			strcat(text, "^");
			strcat(text, obj);
//			fprintf(f3, "%s %s %s %s %s\n",ins.address, ins.label, ins.opcode, ins.operand, obj);
			obj[0] = '\0';
			printf("text record: %s\n", text);
			printf("text length: %d\n", textlen);
		}
		else
		{
			char buffer[5];
			textlen = textlen - strlen(obj)/2;
			printf("text record: %s", text);
//			itoa(textlen, buffer, 16);
			sprintf(buffer,"%x",textlen);
			if(strlen(buffer)==1)
			{
				text[9] = '0';
				text[10] = buffer[0];
			}
			else
			{
				text[9] = buffer[0];
				text[10] = buffer[1];
			}
			fprintf(f2, "%s\n",text);
			text[0] = '\0';
			strcpy(text, "T^00");
			strcat(text, ins.address);
			strcat(text, "^--");
			strcat(text, "^");
			strcat(text, obj);
			textlen = (strlen(obj))/2;
//			fprintf(f3, "%s %s %s %s %s\n",ins.address, ins.label, ins.opcode, ins.operand, obj);
			obj[0] = '\0';
			
		}
		
			
	}
		

	return 0;
}


