#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

	FILE *fp, *mf;
	char *inputFileName = NULL;
	char *outputFileName = NULL;
	char *codeFileName = NULL;

	unsigned char code[256];
	int iout;
	int i, j = 0;

	for (i = 1; i < argc; i += 2) {
		switch (argv[i][1]) {
		case 'i':
			inputFileName = argv[i + 1];
			break;
		case 'o':
			outputFileName = argv[i + 1];
			break;
		case 'z':
			for (j = 0; (j < 256) && (argv[i + 1][j] != 0); j++)
				code[j] = argv[i + 1][j];
			break;
		case 'f':
			codeFileName = argv[i + 1];
			break;
		}
	}

	if (j == 0) {
		printf("Введите пары символов");
		scanf("%s", code);
		for (j = 0; (code[j] != 0) && (j < 128); j++);
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
	if (codeFileName == NULL) {
		codeFileName = (char*)malloc(256);
		printf("Введите файл с парами букв\n");
		scanf("%s", codeFileName);
	}

	fp = fopen(codeFileName, "r");
	if (fp == NULL) {
		printf("Не удалось открыть файл\n");
		exit(1);
	}
	
	while (1)
	{
		iout = fgetc(fp);
		if (feof(fp) != 0) {
			break;
		}
		code[j++] = iout;
	}
	code[j] = 0;
	int codeLen = j;
	fclose(fp);

	fp = fopen(inputFileName, "r");
		if (fp == NULL) {
		printf("Не удалось открыть файл\n");
		exit(1);
	}

	mf = fopen(outputFileName, "w");

	while (1)
	{
		iout = fgetc(fp);
		if (feof(fp) != 0) {
			break;
		}
		for (j = 0; j < codeLen; j++) {
			if ((code[j]) == iout) {
				if ((j % 2) == 0)
					iout = code[j + 1];
				else
					iout = code[j - 1];
				break;
			}
		}

		putc(iout, mf);

	}
	fclose(mf);


	return 0;

}