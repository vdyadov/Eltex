#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int m_s = 5;

void create_matr(int matr[m_s][m_s]){
        for (int i = 0; i < m_s; i++){
            for (int j = 0; j < m_s; j++)
                matr[i][j] = 10 + rand() % 20;
        }
        // Вывод для отладки
        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 5; j++)
                printf("%d  ", matr[i][j]);
  
            printf("\n");
        }
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
long func_mul(int matr[m_s][m_s]){
    long res = 1;

    for (int i = 0; i < m_s; i++){
        for (int j = 0; j < m_s; j++)
            res *= matr[i][j];
    }
    // Вывод для отладки
    // printf("%ld\n", res);
    return res;
}