#include <stdio.h>
#include <conio.h>
#include <string.h>
#define MAX 20
struct estab
{
    char csname[10];
    char extsym[10];
    int address;
    int length;
} es[MAX];

int main()
{
    char input[10], name[10], symbol[10];
    int count = 0, progaddr, csaddr, add, len, flag = 0;
    FILE *fp1, *fp2;
    fp1 = fopen("input.txt", "r");
    fp2 = fopen("output.txt", "w");
    printf("Enter location: ");
    scanf("%d", &progaddr);
    csaddr = progaddr;
    fscanf(fp1, "%s", input);
    while (strcmp(input, "END") != 0)
    {
        if (strcmp(input, "H") == 0)
        {
            fscanf(fp1, "%s", name);
            strcpy(es[count].csname, name);
            // printf("%d \n", count);
            for (int i = 0; i < count; i++)
            {
                if ((strcmp(name, es[i].csname) == 0) && (strcmp(es[i].csname, " ") != 0))
                    printf("Duplicate Found\n");
            }
            strcpy(es[count].extsym, " ");
            fscanf(fp1, "%d", &add);
            es[count].address = add + csaddr;
            fscanf(fp1, "%d", &len);
            es[count].length = len;
            fprintf(fp2, "%s\t%s\t\t%d\t%d\n", es[count].csname, es[count].extsym, es[count].address, es[count].length);
            count++;
        }
        else if (strcmp(input, "D") == 0)
        {
            fscanf(fp1, "%s", input);
            int flag = 0;
            while (strcmp(input, "R") != 0)
            {
                strcpy(es[count].csname, " ");
                strcpy(es[count].extsym, input);
                for (int i = 0; i < count; i++)
                {
                    if (strcmp(es[count].extsym, es[i].extsym) == 0 && (strcmp(es[i].extsym, " ") != 0))
                    {
                        printf("Duplicate Found\n");
                        flag = 1;
                    }
                }
                fscanf(fp1, "%d", &add);
                es[count].address = add + csaddr;
                if (flag == 0)
                    fprintf(fp2, "%s\t%s\t\t%d\t%d\n", es[count].csname, es[count].extsym, es[count].address, es[count].length);
                count++;
                fscanf(fp1, "%s", input);
            }
            csaddr = csaddr + len;
        }
        else if (strcmp(input, "T") == 0)
        {
            while (strcmp(input, "E") != 0)
                fscanf(fp1, "%s", input);
        }
        fscanf(fp1, "%s", input);
    }
    // for(int i=0; i < count; i++)
    // {
    // printf("%s\n", es[i].extsym);
    // }
    fclose(fp1);
    fclose(fp2);
    printf("Completed\n");
    return 0;
}