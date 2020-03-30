#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void doit(){
	pid_t pid, pid2, pid1_1, pid1_2, pid1_3, pid2_1, pid2_2;
	int status;
	pid = fork();
    if (-1 == pid) {
		perror("fork");
		exit(1);
    }
    else if (pid == 0)
    {
        pid1_1 = fork();
        if (-1 == pid1_1) {
		    perror("fork");
		    exit(1);
        }

        if (pid1_1 == 0){
            printf("ПОТОМОК_В: %d\n", getpid());
            exit(3);
        }

        

        pid1_2 = fork();
        if (-1 == pid1_2) {
		    perror("fork");
		    exit(1);
        }
        if (pid1_2 == 0){
            printf("ПОТОМОК_Г: %d\n", getpid());    
            exit(4);
        }

        pid1_3 = fork();

        if (-1 == pid1_3) {
		    perror("fork");
		    exit(1);
        }
        if (pid1_3 == 0){
            printf("ПОТОМОК_Д: %d\n", getpid());
            exit(5);
        }

        printf("ПОТОМОК_А: %d\n", getpid());

        waitpid(pid1_1, &status, 0);
        if (WIFEXITED(status))
            printf("Потомок В завершен. Вернул  %d\n", WEXITSTATUS(status));

        waitpid(pid1_2, &status, 0);
        if (WIFEXITED(status))
            printf("Потомок Г завершен. Вернул  %d\n", WEXITSTATUS(status));

        waitpid(pid1_3, &status, 0);
        if (WIFEXITED(status))
            printf("Потомок Д завершен. Вернул  %d\n", WEXITSTATUS(status));

        exit(1);
    } 
    else
    {
        pid2 = fork();
        if (pid2 == 0){
            pid2_1 = fork();
            if (-1 == pid2_1) {
		        perror("fork");
		        exit(1);
            }
            if (pid2_1 == 0){
                printf("ПОТОМОК_Е: %d\n", getpid());
                exit(6);
            }

            pid2_2 = fork();
            if (-1 == pid2_2) {
		        perror("fork");
		        exit(1);
            }
            if (pid2_2 == 0)
            {
                printf("ПОТОМОК_Ж: %d\n", getpid());
                exit(7);
            }

            printf("ПОТОМОК_Б: %d\n", getpid());
            
            waitpid(pid2_1, &status, 0);
            if (WIFEXITED(status))
                printf("Потомок Е завершен. Вернул  %d\n", WEXITSTATUS(status));

            waitpid(pid2_2, &status, 0);
            if (WIFEXITED(status))
                printf("Потомок Ж завершен. Вернул  %d\n", WEXITSTATUS(status));

            exit(2);
        }
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("Потомок A завершен. Вернул  %d\n", WEXITSTATUS(status));

        waitpid(pid2, &status, 0);
        if (WIFEXITED(status))
            printf("Потомок Б завершен. Вернул  %d\n", WEXITSTATUS(status));

    }
    
}

int main() 
{
    doit();
}