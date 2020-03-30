#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

const int m_s = 5;

void create_matr(int matr[m_s][m_s]){
        for (int i = 0; i < m_s; i++){
            for (int j = 0; j < m_s; j++)
                matr[i][j] = rand() % 20;
        }
        // Вывод для отладки
        /* for (int i = 0; i < 5; i++){
            for (int j = 0; j < 5; j++)
                printf("%d  ", matr[i][j]);
  
            printf("\n");
        } */
}

int func_max(int matr[m_s][m_s]){
    int max = matr[0][0];

    for (int i = 0; i < m_s; i++){
        for (int j = 0; j < m_s; j++)
            max = matr[i][j] > max ? matr[i][j] : max;
    }
    // Вывод для отладки
    // printf("%d\n", max);
    return max;
}
int func_min(int matr[m_s][m_s]){
    int min = matr[0][0];

    for (int i = 0; i < m_s; i++){
        for (int j = 0; j < m_s; j++)
            min = matr[i][j] < min ? matr[i][j] : min;
    }
    // Вывод для отладки
    // printf("%d\n", min);
    return min;
}
int func_sum(int matr[m_s][m_s]){
    int sum = 0;

    for (int i = 0; i < m_s; i++){
        for (int j = 0; j < m_s; j++)
            sum += matr[i][j];
    }
    // Вывод для отладки
    // printf("%d\n", sum);
    return sum;
}
int func_mul(int matr[m_s][m_s]){
    int res = 1;

    for (int i = 0; i < m_s; i++){
        for (int j = 0; j < m_s; j++)
            res *= matr[i][j];
    }
    // Вывод для отладки
    // printf("%ld\n", res);
    return res;
}

void out_fork(int matr[m_s][m_s], int (*f)(int matr[m_s][m_s]), int *o_pipe){
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (-1 == pid){
        perror("Процесс не запущен");
        exit(1);
    }
    if (0 == pid){
        close(fd[0]);
        int res = f(matr);
        write(fd[1], &res, sizeof(int));
        exit(5);
    }else{
        close(fd[1]);
        *o_pipe = fd[0];
    }
}

int main(){
    int matr[m_s][m_s];
    create_matr(matr);

    int max_pipe;
    out_fork(matr, func_max, &max_pipe);
    int min_pipe;
    out_fork(matr, func_min, &min_pipe);
    int sum_pipe;
    out_fork(matr, func_sum, &sum_pipe);
    int mul_pipe;
    out_fork(matr, func_mul, &mul_pipe);
    
    int out;
    read(max_pipe, &out, sizeof(int));
    printf("Максимальное значение матрицы: %d\n", out);
    read(min_pipe, &out, sizeof(int));
    printf("Минимальное значение матрицы: %d\n", out);
    read(sum_pipe, &out, sizeof(int));
    printf("Сумма всех чисел матрицы: %d\n", out);
    read(mul_pipe, &out, sizeof(int));
    printf("Произведение всех чисел матрицы: %d\n", out);

    return 0;
}