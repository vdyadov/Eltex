#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void doit(int argc, char **argv){
    pid_t pid1, pid2 , pid3;
    int status;
    int N = atoi(argv[1]);

    switch (N){
    case 1:
        pid1 = fork();
        if (-1 == pid1){
            perror("Процесс не запущен"); /* произошла ошибка */
		    exit(1);
        }
        if ( pid1 == 0){
            if(execl("./func", "func", argv[1], argv[2], argv[3], NULL) < 0){
                printf("ОШИБКА!!! Файл не открылся\n");
                exit(2);
            }else{
                printf("Программа работает!");
                status = 3;
            }
            exit(status);
        }else{
            waitpid(pid1, &status, 0);
            if (WIFEXITED(status))
                printf("Процесс pid1 завершен. Вернул  %d\n", WEXITSTATUS(status));
        }
        break;
    case 2:
        pid2 = fork();
        if (-1 == pid2){
            perror("Процесс не запущен"); /* произошла ошибка */
		    exit(1);
        }
        if  (pid2 == 0){
            if(execl("./func","func", argv[1], argv[2], NULL) < 0){
                printf("ОШИБКА!!! Файл не открылся\n");
                exit(2);
            }else{
                printf("Программа работает!");
                status = 4;
            }
            exit(status);
        }else{
            waitpid(pid2, &status, 0);
            if (WIFEXITED(status))
                printf("Процесс pid2 завершен. Вернул  %d\n", WEXITSTATUS(status));
        }
        break;
    case 3:
        pid3 = fork();
        if (-1 == pid3){
            perror("Процесс не запущен"); /* произошла ошибка */
		    exit(1);
        }
        if (0 == pid3){
            if(execv("./func", argv) < 0){
                printf("ОШИБКА!!! Файл не открылся\n");
                exit(2);
            }else{
                printf("Программа работает!");
                status = 5;
            }
            exit(status);
        }else{
            waitpid(pid3, &status, 0);
            if (WIFEXITED(status))
                printf("Процесс pid3 завершен. Вернул  %d\n", WEXITSTATUS(status));
        }
    
    default:
        break;
    }
}

int main(int argc, char **argv){
    doit(argc, argv);
}