#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comp(const int * i, const int * j) {
	if (*i > *j) return 1;
	if (*i < *j) return -1;
	return 0;
}

int main(int argc, char **argv){
    int N = atoi(argv[1]);
     
    switch (N){
        case 1:{
            int x = atoi(argv[2]); // кол-во строк
            int y = atoi(argv[3]); // кол-во столбцов

            int **matr = NULL;
            matr = (int**)calloc(x, sizeof(*matr));
            for(int i = 0; i < x; ++i)
                matr[i] = (int*)calloc(y, sizeof(*matr[i]));

            for (int i = 0; i < x; i++){
                for (int j = 0; j < y; j++)
                    printf("%d  ", matr[i][j]);
  
                printf("\n");
            }
        break;
        }
        case 2:{
            for (int i = 2; i < argc; i++)
                printf("%s ", argv[i]);
            printf("\n");
        break;
        }
        case 3:{
            int n = atoi(argv[2]);
            int * arr = (int*)malloc(n * sizeof(int));
            for (int j = 0, i = 3; j < n; j++, i++){
                arr[j] = atoi(argv[i]);}
            qsort(arr, n, sizeof(int), (int(*) (const void *, const void *)) comp);
                for (int j = 0; j < n; j++){
                    printf("%d ", arr[j]);
                }
            printf("\n");
        break;
        }
    default:
        break;
    }
}