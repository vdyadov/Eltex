#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>

#define MAX_SEND_SIZE 80

struct mymsgbuf {
        long mtype;
        char mtext[MAX_SEND_SIZE];
};

int msgqid, rc;

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text){
    qbuf->mtype = type;
    strcpy(qbuf->mtext, text);

    if((msgsnd(qid, (struct msgbuf *)qbuf, strlen(qbuf->mtext)+1, 0)) == -1){
            perror("msgsnd");
            exit(1);
    }
}

void write_massage(int qid, struct mymsgbuf *qbuf, long type){
    qbuf->mtype = type;
    msgrcv(qid, (struct msgbuf *)qbuf, MAX_SEND_SIZE, type, 0);
    printf("%s", /* qbuf->mtype, */ qbuf->mtext);       	
}

int main(int argc, char **argv){
    int status, stat;
    char str[MAX_SEND_SIZE];
    key_t key;
    int type = 1;
    struct mymsgbuf qbuf;

    if (argc < 2){
        printf("Малло аргументов...");
        exit(1);
    }
    
    key = ftok(".", 'm');
    if((msgqid = msgget(key, 0660|IPC_CREAT)) == 1){
        perror("msgget");
        exit(1);
    }

    if (strcmp(argv[1], "-r") == 0){
        pid_t r_pid = fork();

        if (-1 == r_pid){
            perror("fork");
            exit(1);
        }

        if (0 == r_pid){
            printf("Введите текст: \n");
            while (1)
            {
                fgets(str, MAX_SEND_SIZE, stdin);
                send_message(msgqid, (struct mymsgbuf*)&qbuf, type, str);
            }
            printf("Процесс-потмок отправил сообщение");
            fflush(stdout);
            exit(5);
        }

        status = waitpid(r_pid, &stat, 0);
        if (WIFEXITED(status)){
            printf("Процесс потом завершен. Вернул: %d", WEXITSTATUS(status)); 
        }   
    }
    
    if (strcmp(argv[1], "-w") == 0){
        pid_t w_pid = fork();

        if (-1 == w_pid){
            perror("fork");
            exit(1);
        }
        if (0 == w_pid){
            printf("Этот же текст: \n");
            while (1){
                write_massage(msgqid, &qbuf, type);
            }
            exit(5);
        }
        status = waitpid(w_pid, &stat, 0);
        if (WIFEXITED(status)){
            printf("Процесс-потомок завершен. Вернул: %d", WEXITSTATUS(status));
        }
    }
    return 0;
}