#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "funcs.h"

int main(int argc, char **argv){
    int matr[m_s][m_s];
    int *shm, *s, shmid;
    key_t key = atoi(argv[1]);

    int size = sizeof(int) * (m_s * m_s); //размер области разделяемой памяти
    /* подключаемся к области разделяемой памяти */
    if ((shmid = shmget(key, size, 0666)) < 0){
        perror("shmget");
        exit(1);
    }
    /* Получаем доступ к разделяемой памяти */
    if((shm = shmat(shmid,NULL, 0)) == (int*)-1){
        perror("shmat");
        exit(1);
    }
    /* Получаем данные */
    s = shm;
    int t = 0;
    for (int i = 0; i < m_s; i++){
        for (int j = 0; j < m_s; j++){
            matr[i][j] = s[t];
            t++;
        }
    }
    if (strcmp(argv[2], "max") == 0){
        int res = func_max(matr);
        printf("Максимальное значение: %d\n", res);
    }
    if (strcmp(argv[2], "min") == 0){
        int res = func_min(matr);
        printf("Минимальное значение: %d\n", res);
    }
    if (strcmp(argv[2], "sum") == 0){
        int res = func_sum(matr);
        printf("Сумма всех чисел матрицы: %d\n", res);
    }
    if (strcmp(argv[2], "mul") == 0){
        long res = func_mul(matr);
        printf("Произведение всех чисел матрицы: %li\n", res);
    }
    return 0;
}