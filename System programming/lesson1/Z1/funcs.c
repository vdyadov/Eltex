#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void reverse(char *s){
    int length = strlen(s) ;
    int c, i, j;

    for (i = 0, j = length - 1; i < j; i++, j--)
    {
      c = s[i];
      s[i] = s[j];
      s[j] = c;
    }

    printf("%s\n", s);
}

void doit(char* argv){
	pid_t pid;
	int status = 5;
	pid = fork();
    if (-1 == pid) {
		perror("fork");  // произошла ошибка 
		exit(1);
    }
    else if (pid == 0)
    {
        reverse(argv);
        exit(status);
    } 
}