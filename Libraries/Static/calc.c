#include <stdio.h>
#include <stdlib.h>

#include "funcs.h"

#define KEY 3
#define D_SIZE 25

int main() {
  char key[KEY], sx[D_SIZE], sy[D_SIZE];
  double x, y, result;
    
  x = 0.0;
  y = 0.0;
  result = 0.0;

  printf("Для вывода действий на экран введите h\n");
  printf("Введите число\n");
  
  fgets(key, KEY, stdin);

  while (key[0] != 'q') {
    switch  (key[0]) {
      case 'h':
        printf("Для сложения введите 1\n");
        printf("Для вычитания введите 2\n");
        printf("Для умножения введите 3\n");
        printf("Для деления введите 4\n");
        printf("Для выхода введите q\n");
        break;
      case '1':
        printf("Введите 2 числа\n");

        readNum(sx, sy, '+');

        //fgets(sx, D_SIZE, stdin);
        //printf("+\n");
        //fgets(sy, D_SIZE, stdin);
        
        x = atol(sx);
        y = atol(sy);

        result = plus(x, y);

        printf("Ответ: %10.2lf\n", result);
        break;
      case '2':
        printf("Введите 2 числа\n");
        
        readNum(sx, sy, '-');
        
        //fgets(sx, D_SIZE, stdin);
        //printf("-\n");
        //fgets(sy, D_SIZE, stdin);
        
        x = atol(sx);
        y = atol(sy);

        result = minus(x, y);
        printf("Ответ: %10.2lf\n", result);
        break;
      case '3':
        printf("Введите 2 числа\n");
        
        readNum(sx, sy, '*');
        
        //fgets(sx, D_SIZE, stdin);
        //printf("*\n");
        //fgets(sy, D_SIZE, stdin);
        
        x = atol(sx);
        y = atol(sy);
        
        result = multiply(x, y);
        printf("Ответ: %10.2lf\n", result);
        break;
      case '4':
        printf("Введите 2 числа\n");
       
        readNum(sx, sy, '/');
        
        //fgets(sx, D_SIZE, stdin);
        //printf("/\n");
        //fgets(sy, D_SIZE, stdin);
        
        x = atol(sx);
        y = atol(sy);

        result = divide(x, y);
        printf("Ответ: %10.2lf\n", result);
        break;
      case 'q':
        exit(0);
      default:
        printf("Вы ввели неверный символ\n");
      }  
    printf("\nВведите следующую оперцию: ");
    fgets(key, 3, stdin);
    }  

    return 0;
}

void readNum(char *x, char *y, char token) {
  fgets(x, D_SIZE, stdin);
  printf("%c\n", token);
  fgets(y, D_SIZE, stdin);
}
