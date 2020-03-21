#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int * stcount (unsigned char **abs, int *size, int N){

    int *count = NULL; // счетчик    

    /* считаем кол-во небуквенных символов*/
    for (int i = 0; i < N+1; i++)
    {
        count = (int *)realloc(count, (i + 1)* sizeof(int));

        for (int t = 0; abs[i][t] != '\0'; t++)
        {
            if(!(abs[i][t]>='a') && (abs[i][t]<='z') || (abs[i][t]>='A') && (abs[i][t]<='Z') || (abs[i][t]>='а') && (abs[i][t]<='я')||(abs[i][t]>='А')&&(abs[i][t]<='Я'))            {
                
                count[i]++;
            }      
        }   
        (*size)++;
    }    
    return count; 
}

void sortStr (unsigned char **st, int  *count, int N, char *outputFileName){

    for(int j = 0; j < N; ++j)
    {
        printf("%s\n", st[j]);
    }
    printf("\n\n");

    /*сортировка вставками*/
    for (int i = 1; i < N; ++i)
    {
        int k = i; 
        while (k > 0)
        {
            if(count[k-1] > count[k])
            {
                unsigned char* tmp = st[k-1];
                st[k-1] = st[k];
                st[k] = tmp;

                int tmp1 = count[k-1];
                count[k-1] = count[k];
                count[k] = tmp1;
            }
            k -=1;
        }
    }
    
    for(int j = 0; j < N; ++j)
    {
        printf("%d\n", count[j]);
    }

    FILE *fp;

    /*запись в файл*/
    fp = fopen(outputFileName, "w");

    for(int j = 0; j < N; ++j)
    {
        fprintf(fp, "%s", st[j]);
    }

    for(int j = 0; j < N; ++j)
    {
        printf("%s\n", st[j]);
    }


}
