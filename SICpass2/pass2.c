#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main()
{
    FILE *fint, *ftab, *flen, *fsym, *fhead, *fout, *fhex, *ftemp;
    int op1[10], txtlen, txtlen1, i, j = 0, len, count = 0;
    char add[5], symadd[5], op[5], start[10], temp[30], line[20], label[20], mne[10], operand[10], symtab[10], opmne[10];
    char buffer[140], *arr[4], *m, *oprnd, *n_oprnd;
    char *last_token, *delimiter_characters = "", *adr;
    int hex, var1, var2, var3, freq = 0, r = 1;
    char *store[10];
    char st[50];

    fint = fopen("intermidiate.txt", "r");
    flen = fopen("length.txt", "r");
    fsym = fopen("symtab.txt", "r");
    fhead = fopen("objectprogram.txt", "w");
    fout = fopen("output.txt", "w");
    fhex = fopen("hex.txt", "w");
    ftemp = fopen("temp.txt", "w");
    fscanf(fint, "%s%s%s%s", add, label, mne, operand); // reads first line from int file

    if (strcmp(mne, "START") == 0)
    {
        fprintf(fout, "$\t%s\t%s\t%s\n", label, mne, operand); // writes first line to output file
        strcpy(start, operand);                                // stores operand value as starting address
        fscanf(flen, "%d", &len);                              // total length of prgm
    }
    fprintf(fhead, "H^%6s^00%s^0000%d\n", label, start, len); // writes header record
    printf("\nHeader record written successfully\n");
    fclose(fint);

    fprintf(fhead, "T"); // Initialize first text record

    fint = fopen("intermidiate.txt", "r");
    fgets(buffer, 140, fint); // reads length of 140 and store to buffer from int
    fclose(fsym);

    while (!feof(fint))
    {
        count = 0;
        fgets(buffer, 140, fint); // reads next line

        last_token = strtok(buffer, " "); // tokenize
        adr = last_token;
        while (last_token != NULL)
        {
            arr[count] = last_token; // counts tokens in a line and stores in a array
            count++;
            last_token = strtok(NULL, " ");
        }

        if (count == 3) // 3 words, locctr, opcode, oprnd
        {
            m = arr[1];                 // opcode
            oprnd = arr[2];             // operand
            oprnd = strtok(oprnd, ","); // incase buffer,x tokenize again
        }
        else if (count == 4) // 4 words extra label
        {
            m = arr[2];     // mne
            oprnd = arr[3]; // operand constant
            oprnd = strtok(oprnd, ",");
        }
        else
            break; // from text record

        if (r == 1) // starts new text record
        {
            fprintf(fhead, "^%s", adr); // copies the starting address
            r = 0;
        }

        ftab = fopen("optab.txt", "r");
        oprnd = strtok(oprnd, "\n");
        fscanf(ftab, "%s%s", opmne, op); // reads line from optab

        if (strcmp(m, "WORD") == 0)
        {
            hex = strtol(oprnd, NULL, 10);
            if (hex >= 4096)
                fprintf(ftemp, "^00%x", hex);
            else if (hex >= 256)
                fprintf(ftemp, "^00000%x", hex);
            else if (hex >= 64)
                fprintf(ftemp, "^0000%x", hex);
            else if (hex >= 16)
                fprintf(ftemp, "^00000%x", hex);
            else
                fprintf(ftemp, "^00000%x", hex);
            freq++;
        }
        else if (strcmp(m, "BYTE") == 0)
        {
            char tem[] = "C";
            // int len=strlen(oprnd)-1;
            char *nn_oprnd = strtok(oprnd, "'"); // tokenize to get c or x
            if (strcmp(nn_oprnd, tem) == 0)
            {
                nn_oprnd = strtok(NULL, "'"); // tokenize

                for (int i = 0; i < strlen(nn_oprnd); i++)
                    fprintf(fhex, "%d\t", nn_oprnd[i]); // stores ascii value each char

                fclose(fhex);
                fhex = fopen("hex.txt", "r");
                fscanf(fhex, "%d%d%d", &var1, &var2, &var3); // reads the stored ascii value
                fprintf(ftemp, "^%x%x%x", var1, var2, var3); // converts to hex and store in temp file
                freq++;
                fclose(fhex);
            }
            else // for X
            {
                nn_oprnd = strtok(NULL, "'");
                // reads the stored ascii value
                fprintf(ftemp, "^%s", nn_oprnd); // store in temp file
                freq++;
                fclose(fhex);
            }
        }
        else if (strcmp(m, "RESB") != 0 && strcmp(m, "RESW") != 0)
        {
            int found1 = 0;
            while (!feof(ftab))
            {
                if (strcmp(m, opmne) == 0) // optab mne and m
                {
                    fclose(ftab);
                    found1 = 1;
                    fsym = fopen("symtab.txt", "r"); // optab, search address of a symbol in symtab
                    int found = 0;
                    fscanf(fsym, "%s%s", symtab, symadd);
                    while (!feof(fsym))
                    {
                        if (strcmp(symtab, oprnd) == 0)
                        {
                            fclose(fsym);
                            fprintf(ftemp, "^%s%s", op, symadd); // opcode+symadd
                            found = 1;
                            freq++;
                            break;
                        }
                        else
                            fscanf(fsym, "%s%s", symtab, symadd); // reads next line
                    }
                    if (found == 0)
                        printf("\nSymbol not found\n\n");
                    break;
                }
                else
                    fscanf(ftab, "%s%s", opmne, op); // search for optab
            }
            if (found1 == 0)
                printf("\nOpcode not found\n\n");
        }
        else
        {
            fclose(ftemp);
            ftemp = fopen("temp.txt", "r");
            // fscanf(ftemp,"%s",st);
            fgets(st, 50, ftemp); // reads every line to st
            // printf("%s",st);
            fprintf(fhead, "^%x%s", freq * 3, st); // writes every line from st to obj prgm
            fprintf(fhead, "\nT");                 // initialises next text record after break
            fclose(ftemp);
            ftemp = fopen("temp.txt", "w+"); // for new text record
            freq = 0;
            r = 1;
        }

        if (freq == 10)
        {
            fclose(ftemp);
            ftemp = fopen("temp.txt", "r");
            // fscanf(ftemp,"%s",st);
            fgets(st, 50, ftemp);
            fprintf(fhead, "^%x%6s", freq * 3, st);
            fprintf(fhead, "\nT");
            freq = 0;
            r = 1;
        }
    }
    fclose(ftemp);
    ftemp = fopen("temp.txt", "r");
    // ftemp=fopen("temp.txt","r");
    // fscanf(ftemp,"%s",st);
    fgets(st, 50, ftemp); // remaining last records
    fprintf(fhead, "^%x%s", freq * 3, st);
    printf("\nText record written successfully\n");
    fprintf(fhead, "\nE^00%s", start);
    printf("\nEnd record written successfully\n\n");

    return 0;
}