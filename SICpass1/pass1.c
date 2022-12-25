#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *fp;
FILE *iof;
FILE *sof;
int label_count = 0, opcode_count = 0, operand_count = 0;
int line = 0;
char label[100][100], opcode[100][100], operand[100][100];

int count_words(char *str)
{
    int OUT = 0, IN = 1;
    int state = OUT;
    int wc = 0;
    while (*str)
    {
        if (*str == ' ' || *str == '\n' || *str == '\t')
            state = OUT;
        else if (state == OUT)
        {
            state = IN;
            ++wc;
        }
        ++str;
    }
    return wc;
}

long pass(FILE *iof, FILE *sof, char *buf, long loc)
{
    char ofile[300];
    // printf("%x\n\n", loc);
    int k = count_words(buf);
    int rw = 0, rb = 0, by = 0, nn = 0;
    strcpy(ofile, buf);

    fprintf(iof, "%x\t%s\n", loc, ofile);

    // int k = count_words(buf);
    char *token = strtok(buf, " ");
    if (k == 1)
    {
        while (token != NULL)
        {
            // printf("%s\n", token);
            strcpy(opcode[opcode_count], token);
            opcode_count++;
            token = strtok(NULL, " ");
        }
    }
    else if (k == 2)
    {
        // printf("Hello\n\n");
        int h = 0;
        while (token != NULL)
        {
            // printf("%s\n", token);
            if (h == 0)
            {
                strcpy(opcode[opcode_count], token);
                if (strcmp(token, "BYTE") == 0)
                {
                    by = 1;
                }
                else if (strcmp(token, "RESW") == 0)
                {
                    rw = 1;
                }
                else if (strcmp(token, "RESB") == 0)
                {
                    rb = 1;
                }
                else
                {
                    nn = 1;
                }

                opcode_count++;
            }
            else if (h == 1)
            {
                char *rem;
                long add;
                strcpy(operand[operand_count], token);
                if (rw == 1)
                {
                    add = strtol(token, &rem, 16);
                    loc = loc + (3 * add);
                }
                else if (rb == 1)
                {
                    add = strtol(token, &rem, 10);
                    loc = loc + add;
                }
                else if (by == 1)
                {
                    if (token[0] == 'C')
                    {
                        add = strlen(token);
                        // printf("\nstr len %d\n", add);
                        loc = loc + (add - 4);
                    }
                    else if (token[0] == 'X')
                    {
                        add = strlen(token);
                        loc = loc + ((add - 4) / 2);
                    }
                }
                operand_count++;
            }
            h++;
            token = strtok(NULL, " ");
        }
    }
    else if (k == 3)
    {
        int h = 0;
        while (token != NULL)
        {
            // printf("%s\n", token);
            if (h == 0)
            {
                strcpy(label[label_count], token);
                int f = 0;
                for (int i = 0; i < label_count; i++)
                {
                    if (strcmp(label[i], token) == 0)
                    {
                        f = 1;
                    }
                }
                if (f == 0)
                {
                    fprintf(sof, "%s\t%x\n", token, loc);
                }
                else if (f == 1)
                {
                    // fprintf(sof, "Dupicate is found at %dth line\n", line);
                    printf("Dupicate is found at line %d\n\n", line);
                }

                label_count++;
            }
            else if (h == 1)
            {
                strcpy(opcode[opcode_count], token);
                if (strcmp(token, "BYTE") == 0)
                {
                    by = 1;
                }
                else if (strcmp(token, "RESW") == 0)
                {
                    rw = 1;
                }
                else if (strcmp(token, "RESB") == 0)
                {
                    rb = 1;
                }
                else
                {
                    nn = 1;
                }
                opcode_count++;
            }
            else if (h == 2)
            {
                char *rem;
                int add;
                strcpy(operand[operand_count], token);
                if (rw == 1)
                {
                    add = strtol(token, &rem, 16);
                    loc = loc + (3 * add);
                }
                else if (rb == 1)
                {
                    add = strtol(token, &rem, 10);
                    loc = loc + add;
                }
                else if (by == 1)
                {
                    if (token[0] == 'C')
                    {
                        add = strlen(token);
                        // printf("\nstr len %d\n", add);
                        // printf("%s", token);
                        loc = loc + (add - 4);
                    }
                    else if (token[0] == 'X')
                    {
                        add = strlen(token);
                        loc = loc + ((add - 4) / 2);
                    }
                }
                operand_count++;
            }

            h++;
            token = strtok(NULL, " ");
        }
    }
    if (nn == 1)
    {
        loc = loc + 3;
    }
    return loc;
}

int main()
{
    char filename[20] = "input.txt";
    char buf[200];
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Unable to open file\n");
        exit(0);
    }
    else
    {
        long int loc, iloc;
        char ofile[300];
        iof = fopen("intermediate.txt", "w+");
        if (iof == NULL)
        {
            printf("unable to create intermediatre table\n");
        }
        sof = fopen("symtab.txt", "w");
        if (sof == NULL)
        {
            printf("unable to create Symbol table\n");
        }
        while (fgets(buf, sizeof(buf), fp))
        {
            // int k = count_words(buf);
            char addr[300] = "0000";

            if (line == 0)
            {
                char temp[300];
                strcpy(temp, buf);
                char *toke = strtok(temp, " ");
                int h = 0;
                while (toke != NULL)
                {
                    // printf("%s\n", token);
                    char init[300];
                    if ((strcmp(toke, "Start") == 0))
                    {
                        strcpy(init, toke);
                    }

                    if (h == 2 && (strcmp(init, "Start") == 0))
                    {
                        strcpy(addr, toke);
                        fprintf(iof, "%s\t%s\n", addr, buf);
                        char *rem;
                        iloc = strtol(addr, &rem, 16);
                        loc = iloc;
                    }
                    else
                    {
                        char *rem;
                        iloc = strtol(addr, &rem, 16);
                        loc = iloc;
                        loc = pass(iof, sof, buf, loc);
                    }
                    h++;
                    toke = strtok(NULL, " ");
                }
                // char *rem;
                // iloc = strtol(addr, &rem, 16);
                // loc = iloc;
                // printf("%x", iloc);
            }
            // printf("%s", buf);
            else
            {
                loc = pass(iof, sof, buf, loc);
            }
            line++;
        }
        printf("Length of the code %x", (loc - iloc));
    }
    fclose(sof);
    fclose(iof);
    fclose(fp);

    return 0;
}