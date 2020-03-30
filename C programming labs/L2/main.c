#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>

#include "Header.h"

int main(int argc, char* argv[]){

    const int n = 1024;
    char *inputFileName = NULL;
	char *outputFileName = NULL;
    unsigned char str[n];
    unsigned char *pstr;
    unsigned char *abs[n];
    int size = 0;
    int c = 0;
    int i = 0;

    for (i = 1; i < argc; i += 2) {
		switch (argv[i][1]) {
		case 'i':
			inputFileName = argv[i + 1];
			break;
		case 'o':
			outputFileName = argv[i + 1];
			break;
        }
    }

    if (inputFileName == NULL) {
		inputFileName = (char*)malloc(256);
		printf("Введите Файл с текстом\n");
		scanf("%s", inputFileName);
	}
	if (outputFileName == NULL) {
		outputFileName = (char*)malloc(256);
		printf("Введите файл, куда записывать данные\n");
		scanf("%s", outputFileName);
    } 

    FILE *fp;
    fp = fopen(inputFileName, "r");
	if (fp == NULL) {
		printf("Не удалось открыть файл\n");
		exit(1);
	}

    for (i = 0; !feof(fp) && i < n; i++)
    {
        pstr = fgets(str, n, fp); // читаем в массив str

        int len = strlen(str); // определяем длинну строки

        abs[i] = (char * )malloc((len + 1) * sizeof(char)); // выделяем память

        strcpy(abs[i], str); // записываем все что прочитали в массив

        c += 1; // считаем кол-во элементов массива
    }   

    int arrSize = c-1;

    int * count = stcount(abs,&size, arrSize);

    fclose(fp);

    sortStr(abs, count, arrSize, outputFileName);

    return 0;
}