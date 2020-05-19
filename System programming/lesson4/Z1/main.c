#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "funcs.h"

const char *flags[] = {"max", "min", "sum", "mul"};

int main(int argc, char **argv){
    int matr[m_s][m_s];
    create_matr(matr);
    int *shm, *s, shmid;
    key_t key;

    int size = sizeof(int) * (m_s * m_s); //размер области разделяемой памяти
    if ((key = ftok(".", 'S')) < 0){
        perror("Невозможно создать ключ");
        exit(1);
    }
    /* создаем область разделяемой памяти */
    if ((shmid = shmget(key, size, IPC_CREAT|0666)) < 0){
        perror("shmget");
        exit(1);
    }
    /* Получаем доступ к разделяемой памяти */
    if((shm = shmat(shmid, NULL, 0)) == (int*)-1){
        perror("shmat");
        exit(1);
    }
    /* Добавляем в память */
    s = shm;
    int t = 0;
    for (int i = 0; i < m_s; i++){
        for (int j = 0; j < m_s; j++){
            s[t] = matr[i][j];
            t++;
        }
    }
    
    for (int i = 0; i < 4; i++){
        char nkey[64];
        sprintf(nkey, "%d", key);
        pid_t pid = fork();
        if (-1 == pid){
            printf("Процеес не создан");
        }
        if (0 == pid){
            int rc;
            if((rc = execl("./fork", "fork", nkey, flags[i], NULL)) < 0){
                perror("exec");
            }
            exit(5);
        }
    }

    wait(NULL);

    if ((shmctl(shmid, IPC_RMID, 0)) < 0){
        perror("shmctl");
        exit(1);
    }
    return 0;
}
